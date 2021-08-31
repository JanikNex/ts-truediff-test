#include <cstring>
#include "gtest/gtest.h"

extern "C" {
#include "libs/tree-sitter/includes/parser.h"
#include "libs/tree-sitter/includes/api.h"
#include "libs/debug.h"
#include "libs/util.h"
TSLanguage *tree_sitter_exp();
}

static const TSLanguage *lang = tree_sitter_exp();

class SimpleExpFixture : public ::testing::Test {
public:
  SimpleExpFixture() {
    // Create a parser.
    parser = ts_parser_new();

    // Set the parser's language.
    ts_parser_set_language(parser, lang);

    lit_map = ts_literal_map_create(lang);
    ts_literal_map_add_literal(lit_map, 10); // number
    ts_literal_map_add_literal(lit_map, 22); // string
  }

protected:
  ~SimpleExpFixture() override {
    ts_literal_map_destroy(lit_map);
    ts_parser_delete(parser);
  }

  TSParser *parser;
  TSLiteralMap *lit_map;
};

static inline void
test_edit_script(const char *code1, const char *code2, int expected_edits, TSParser *parser, TSLiteralMap *lit_map) {
  TSTree *tree = ts_parser_parse_string(
    parser,
    nullptr,
    code1,
    strlen(code1)
  );

  TSTree *tree2 = ts_parser_parse_string(
    parser,
    nullptr,
    code2,
    strlen(code2)
  );
  ts_diff_heap_initialize(tree, code1, lit_map);
  ts_diff_heap_initialize(tree2, code2, lit_map);
  TSNode source_root2 = ts_tree_root_node(tree2);

  TSDiffResult diff_result = ts_compare_to(tree, tree2, code1, code2, lit_map);
  TSNode constructed_root = ts_tree_root_node(diff_result.constructed_tree);

  printf("===== Sequential EditScript =====\n");
  print_edit_script(tree->language, diff_result.edit_script);

  EXPECT_FALSE(ts_reconstruction_test(source_root2, constructed_root));
  EXPECT_EQ(ts_edit_script_length(diff_result.edit_script), expected_edits);
  ts_edit_script_delete(diff_result.edit_script);
  ts_diff_heap_delete(tree);
  ts_diff_heap_delete(tree2);
  ts_diff_heap_delete(diff_result.constructed_tree);
  ts_tree_delete(tree);
  ts_tree_delete(tree2);
  ts_tree_delete(diff_result.constructed_tree);
}


TEST_F(SimpleExpFixture, leave_subtrees_in_place_1) {
  const char *source = "Add(Num(10), Num(13))";
  const char *dest = "Add(Num(13), Num(13))";
  test_edit_script(source, dest, 1, parser, lit_map);
}

TEST_F(SimpleExpFixture, leave_subtrees_in_place_2) {
  const char *source = "Add(Num(13), Num(10))";
  const char *dest = "Add(Num(13), Num(13))";
  test_edit_script(source, dest, 1, parser, lit_map);
}

TEST_F(SimpleExpFixture, leave_subtrees_intact_1) {
  const char *source = "Add(Num(1), Add(Num(3), Add(Num(2), Num(3))))";
  const char *dest = "Add(Num(2), Add(Num(2), Num(3)))";
  test_edit_script(source, dest, 14, parser, lit_map);
}

TEST_F(SimpleExpFixture, leave_subtrees_intact_2) {
  const char *source = "Add(Num(1), Add(Num(3), Add(Num(2), Num(3))))";
  const char *dest = "Add(Add(Num(2), Add(Num(0), Num(0))), Add(Num(2), Num(3)))";
  test_edit_script(source, dest, 19, parser, lit_map);
}

TEST_F(SimpleExpFixture, reuse_all_subtrees_0) {
  const char *source = "Add(Add(Num(1), Num(2)), Num(5))";
  const char *dest = "Add(Num(5), Add(Num(1), Num(2)))";
  test_edit_script(source, dest, 8, parser, lit_map);
}

TEST_F(SimpleExpFixture, reuse_all_subtrees_1) {
  const char *source = "Add(Add(Num(2), Num(3)), Add(Num(2), Num(3)))";
  const char *dest = "Add(Add(Num(2), Num(3)), Num(2))";
  test_edit_script(source, dest, 13, parser, lit_map);
}

TEST_F(SimpleExpFixture, reuse_all_subtrees_2) {
  const char *source = "Add(Add(Num(2), Num(3)), Add(Num(2), Num(3)))";
  const char *dest = "Add(Num(2), Add(Num(2), Num(3)))";
  test_edit_script(source, dest, 11, parser, lit_map);
}

TEST_F(SimpleExpFixture, reuse_all_subtrees_3) {
  const char *source = "Add(Num(2), Add(Num(3), Add(Num(2), Num(3))))";
  const char *dest = "Add(Add(Num(2), Num(3)), Add(Num(2), Num(3)))";
  test_edit_script(source, dest, 10, parser, lit_map);
}

TEST_F(SimpleExpFixture, wrap_subtrees_1) {
  const char *source = "Add(Num(2), Num(3))";
  const char *dest = "Add(Num(1), Add(Num(2), Num(3)))";
  test_edit_script(source, dest, 13, parser, lit_map);
}

TEST_F(SimpleExpFixture, warp_subtrees_2) {
  const char *source = "Add(Num(2), Num(3))";
  const char *dest = "Add(Add(Num(2), Num(3)), Add(Num(2), Num(3)))";
  test_edit_script(source, dest, 21, parser, lit_map);
}

TEST_F(SimpleExpFixture, swap_updates) {
  const char *source = "Add(Num(2), Num(3))";
  const char *dest = "Add(Num(3), Num(2))";
  test_edit_script(source, dest, 2, parser, lit_map);
}
