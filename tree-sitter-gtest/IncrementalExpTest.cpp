#include <cstring>
#include "libs/tree-sitter/includes/parser.h"
#include "libs/tree-sitter/includes/api.h"
#include "libs/debug.h"
#include "gtest/gtest.h"

extern "C" TSLanguage *tree_sitter_exp();

static const TSLanguage *lang = tree_sitter_exp();

class IncrementalExpFixture : public ::testing::Test {
public:
  IncrementalExpFixture() {
    // Create a parser.
    parser = ts_parser_new();

    // Set the parser's language.
    ts_parser_set_language(parser, lang);

    lit_map = ts_literal_map_create(lang);
    ts_literal_map_add_literal(lit_map, 10); // number
    ts_literal_map_add_literal(lit_map, 22); // string
  }

protected:
  ~IncrementalExpFixture() override {
    ts_literal_map_destroy(lit_map);
    ts_parser_delete(parser);
  }

  TSParser *parser;
  TSLiteralMap *lit_map;
};


testing::AssertionResult
inc_edit_script_size_test(int edit_script_size, int expected_normal_edits, int expected_inc_edits) {
  if (edit_script_size == expected_normal_edits || edit_script_size == expected_inc_edits) {
    return testing::AssertionSuccess();
  }
  return testing::AssertionFailure() << edit_script_size << " does not match the expected edits "
                                     << expected_normal_edits << " or " << expected_inc_edits;
};

static inline void test_normal_execution(const char *code1, const char *code2, int expected_edits, TSParser *parser,
                                         TSLiteralMap *lit_map) {
  // NORMAL EXECUTION
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
  TSNode reconstructed_root = ts_tree_root_node(diff_result.constructed_tree);
  printf("===== Sequential EditScript =====\n");
  print_edit_script(tree->language,diff_result.edit_script);
  EXPECT_FALSE(ts_reconstruction_test(source_root2, reconstructed_root));
  EXPECT_EQ(ts_edit_script_length(diff_result.edit_script), expected_edits);

  ts_edit_script_delete(diff_result.edit_script);
  ts_diff_heap_delete(tree);
  ts_tree_delete(tree);
  ts_diff_heap_delete(tree2);
  ts_tree_delete(tree2);
  ts_diff_heap_delete(diff_result.constructed_tree);
  ts_tree_delete(diff_result.constructed_tree);
}

static inline void
test_incremental_execution(const char *code1, const char *code2, int expected_edits_normal,
                           int expected_edits_incremental, int edit_count, TSInputEdit *edits,
                           TSParser *parser, TSLiteralMap *lit_map) {
  // INCREMENTAL EXECUTION
  TSTree *tree = ts_parser_parse_string(
    parser,
    nullptr,
    code1,
    strlen(code1)
  );

  ts_diff_heap_initialize(tree, code1, lit_map);

  for (int i = 0; i < edit_count; i++) {
    ts_tree_edit(tree, &edits[i]);
  }

  TSTree *tree2 = ts_parser_parse_string(
    parser,
    tree,
    code2,
    strlen(code2)
  );

  ts_diff_heap_initialize(tree2, code2, lit_map);
  TSNode tree_root2 = ts_tree_root_node(tree2);

  TSDiffResult diff_result = ts_compare_to(tree, tree2, code1, code2, lit_map);
  TSNode reconstructed_root = ts_tree_root_node(diff_result.constructed_tree);
  printf("===== Incremental EditScript =====\n");
  print_edit_script(tree->language,diff_result.edit_script);
  EXPECT_FALSE(ts_reconstruction_test(tree_root2, reconstructed_root));
  EXPECT_TRUE(inc_edit_script_size_test(ts_edit_script_length(diff_result.edit_script), expected_edits_normal, expected_edits_incremental));

  ts_edit_script_delete(diff_result.edit_script);
  ts_diff_heap_delete(tree);
  ts_tree_delete(tree);
  ts_diff_heap_delete(tree2);
  ts_tree_delete(tree2);
  ts_diff_heap_delete(diff_result.constructed_tree);
  ts_tree_delete(diff_result.constructed_tree);
}

static inline void
test_edit_script(const char *code1, const char *code2, int expected_edits_normal, int expected_edits_incremental,
                 int edit_count, TSInputEdit *edits,
                 TSParser *parser, TSLiteralMap *lit_map) {
  test_normal_execution(code1, code2, expected_edits_normal, parser, lit_map);
  test_incremental_execution(code1, code2, expected_edits_normal, expected_edits_incremental, edit_count, edits,
                             parser, lit_map);
}


TEST_F(IncrementalExpFixture, leave_subtrees_in_place_1) {
  const char *source = "Add(Num(10), Num(13))";
  const char *dest = "Add(Num(13), Num(13))";
  const int edit_count = 1;
  TSInputEdit edits[edit_count];
  edits[0] = (TSInputEdit) {
    .start_byte=9,
    .old_end_byte=10,
    .new_end_byte=10,
    .start_point={.row=0, .column=9},
    .old_end_point={.row=0, .column=10},
    .new_end_point={.row=0, .column=10}
  };
  test_edit_script(source, dest, 1, 1, edit_count, edits, parser, lit_map);
}

TEST_F(IncrementalExpFixture, leave_subtrees_in_place_2) {
  const char *source = "Add(Num(13), Num(10))";
  const char *dest = "Add(Num(13), Num(13))";
  const int edit_count = 1;
  TSInputEdit edits[edit_count];
  edits[0] = (TSInputEdit) {
    .start_byte=18,
    .old_end_byte=19,
    .new_end_byte=19,
    .start_point={.row=0, .column=18},
    .old_end_point={.row=0, .column=19},
    .new_end_point={.row=0, .column=19}
  };
  test_edit_script(source, dest, 1, 1, edit_count, edits, parser, lit_map);
}

TEST_F(IncrementalExpFixture, leave_subtrees_intact_1) {
  const char *source = "Add(Num(1), Add(Num(3), Add(Num(2), Num(3))))";
  const char *dest = "Add(Num(2), Add(Num(2), Num(3)))";
  const int edit_count = 2;
  TSInputEdit edits[edit_count];
  edits[0] = (TSInputEdit) {
    .start_byte=8,
    .old_end_byte=21,
    .new_end_byte=9,
    .start_point={.row=0, .column=8},
    .old_end_point={.row=0, .column=21},
    .new_end_point={.row=0, .column=9}
  };
  edits[1] = (TSInputEdit) {
    .start_byte=32,
    .old_end_byte=33,
    .new_end_byte=32,
    .start_point={.row=0, .column=32},
    .old_end_point={.row=0, .column=33},
    .new_end_point={.row=0, .column=32}
  };
  test_edit_script(source, dest, 14, 14, edit_count, edits, parser, lit_map);
}

TEST_F(IncrementalExpFixture, leave_subtrees_intact_2) {
  const char *source = "Add(Num(1), Add(Num(3), Add(Num(2), Num(3))))";
  const char *dest = "Add(Add(Num(2), Add(Num(0), Num(0))), Add(Num(2), Num(3)))";
  const int edit_count = 4;
  TSInputEdit edits[edit_count];
  edits[0] = (TSInputEdit) {
    .start_byte=0,
    .old_end_byte=0,
    .new_end_byte=4,
    .start_point={.row=0, .column=0},
    .old_end_point={.row=0, .column=0},
    .new_end_point={.row=0, .column=4}
  };
  edits[1] = (TSInputEdit) {
    .start_byte=12,
    .old_end_byte=13,
    .new_end_byte=13,
    .start_point={.row=0, .column=12},
    .old_end_point={.row=0, .column=13},
    .new_end_point={.row=0, .column=13}
  };
  edits[2] = (TSInputEdit) {
    .start_byte=24,
    .old_end_byte=25,
    .new_end_byte=35,
    .start_point={.row=0, .column=24},
    .old_end_point={.row=0, .column=25},
    .new_end_point={.row=0, .column=35}
  };
  edits[3] = (TSInputEdit) {
    .start_byte=58,
    .old_end_byte=59,
    .new_end_byte=58,
    .start_point={.row=0, .column=58},
    .old_end_point={.row=0, .column=59},
    .new_end_point={.row=0, .column=58}
  };
  test_edit_script(source, dest, 19, 20, edit_count, edits, parser, lit_map);
}

TEST_F(IncrementalExpFixture, reuse_all_subtrees_0) {
  const char *source = "Add(Add(Num(1), Num(2)), Num(5))";
  const char *dest = "Add(Num(5), Add(Num(1), Num(2)))";
  const int edit_count = 3;
  TSInputEdit edits[edit_count];
  edits[0] = (TSInputEdit) {
    .start_byte=4,
    .old_end_byte=4,
    .new_end_byte=12,
    .start_point={.row=0, .column=4},
    .old_end_point={.row=0, .column=4},
    .new_end_point={.row=0, .column=12}
  };
  edits[1] = (TSInputEdit) {
    .start_byte=31,
    .old_end_byte=38,
    .new_end_byte=31,
    .start_point={.row=0, .column=31},
    .old_end_point={.row=0, .column=38},
    .new_end_point={.row=0, .column=31}
  };
  edits[2] = (TSInputEdit) {
    .start_byte=32,
    .old_end_byte=33,
    .new_end_byte=32,
    .start_point={.row=0, .column=32},
    .old_end_point={.row=0, .column=33},
    .new_end_point={.row=0, .column=32}
  };
  test_edit_script(source, dest, 8, 8, edit_count, edits, parser, lit_map);
}

TEST_F(IncrementalExpFixture, reuse_all_subtrees_1) {
  const char *source = "Add(Add(Num(2), Num(3)), Add(Num(2), Num(3)))";
  const char *dest = "Add(Add(Num(2), Num(3)), Num(2))";
  const int edit_count = 3;
  TSInputEdit edits[edit_count];
  edits[0] = (TSInputEdit) {
    .start_byte=25,
    .old_end_byte=29,
    .new_end_byte=25,
    .start_point={.row=0, .column=25},
    .old_end_point={.row=0, .column=29},
    .new_end_point={.row=0, .column=25}
  };
  edits[1] = (TSInputEdit) {
    .start_byte=31,
    .old_end_byte=38,
    .new_end_byte=31,
    .start_point={.row=0, .column=31},
    .old_end_point={.row=0, .column=38},
    .new_end_point={.row=0, .column=31}
  };
  edits[2] = (TSInputEdit) {
    .start_byte=32,
    .old_end_byte=34,
    .new_end_byte=32,
    .start_point={.row=0, .column=32},
    .old_end_point={.row=0, .column=34},
    .new_end_point={.row=0, .column=32}
  };
  test_edit_script(source, dest, 13, 13, edit_count, edits, parser, lit_map);
}

TEST_F(IncrementalExpFixture, reuse_all_subtrees_2) {
  const char *source = "Add(Add(Num(2), Num(3)), Add(Num(2), Num(3)))";
  const char *dest = "Add(Num(2), Add(Num(2), Num(3)))";
  const int edit_count = 2;
  TSInputEdit edits[edit_count];
  edits[0] = (TSInputEdit) {
    .start_byte=0,
    .old_end_byte=4,
    .new_end_byte=0,
    .start_point={.row=0, .column=0},
    .old_end_point={.row=0, .column=4},
    .new_end_point={.row=0, .column=0}
  };
  edits[1] = (TSInputEdit) {
    .start_byte=9,
    .old_end_byte=18,
    .new_end_byte=9,
    .start_point={.row=0, .column=9},
    .old_end_point={.row=0, .column=18},
    .new_end_point={.row=0, .column=9}
  };
  test_edit_script(source, dest, 11, 13, edit_count, edits, parser, lit_map);
}

TEST_F(IncrementalExpFixture, reuse_all_subtrees_3) {
  const char *source = "Add(Num(2), Add(Num(3), Add(Num(2), Num(3))))";
  const char *dest = "Add(Add(Num(2), Num(3)), Add(Num(2), Num(3)))";
  const int edit_count = 4;
  TSInputEdit edits[edit_count];
  edits[0] = (TSInputEdit) {
    .start_byte=0,
    .old_end_byte=0,
    .new_end_byte=4,
    .start_point={.row=0, .column=0},
    .old_end_point={.row=0, .column=0},
    .new_end_point={.row=0, .column=4}
  };
  edits[1] = (TSInputEdit) {
    .start_byte=16,
    .old_end_byte=20,
    .new_end_byte=16,
    .start_point={.row=0, .column=16},
    .old_end_point={.row=0, .column=20},
    .new_end_point={.row=0, .column=16}
  };
  edits[2] = (TSInputEdit) {
    .start_byte=21,
    .old_end_byte=21,
    .new_end_byte=22,
    .start_point={.row=0, .column=21},
    .old_end_point={.row=0, .column=21},
    .new_end_point={.row=0, .column=22}
  };
  edits[3] = (TSInputEdit) {
    .start_byte=45,
    .old_end_byte=46,
    .new_end_byte=45,
    .start_point={.row=0, .column=45},
    .old_end_point={.row=0, .column=46},
    .new_end_point={.row=0, .column=45}
  };
  test_edit_script(source, dest, 10, 14, edit_count, edits, parser, lit_map);
}

TEST_F(IncrementalExpFixture, wrap_subtrees_1) {
  const char *source = "Add(Num(2), Num(3))";
  const char *dest = "Add(Num(1), Add(Num(2), Num(3)))";
  const int edit_count = 2;
  TSInputEdit edits[edit_count];
  edits[0] = (TSInputEdit) {
    .start_byte=0,
    .old_end_byte=0,
    .new_end_byte=12,
    .start_point={.row=0, .column=0},
    .old_end_point={.row=0, .column=0},
    .new_end_point={.row=0, .column=12}
  };
  edits[1] = (TSInputEdit) {
    .start_byte=31,
    .old_end_byte=31,
    .new_end_byte=32,
    .start_point={.row=0, .column=31},
    .old_end_point={.row=0, .column=31},
    .new_end_point={.row=0, .column=32}
  };
  test_edit_script(source, dest, 13, 13, edit_count, edits, parser, lit_map);
}

TEST_F(IncrementalExpFixture, wrap_subtrees_2) {
  const char *source = "Add(Num(2), Num(3))";
  const char *dest = "Add(Add(Num(2), Num(3)), Add(Num(2), Num(3)))";
  const int edit_count = 2;
  TSInputEdit edits[edit_count];
  edits[0] = (TSInputEdit) {
    .start_byte=0,
    .old_end_byte=0,
    .new_end_byte=4,
    .start_point={.row=0, .column=0},
    .old_end_point={.row=0, .column=0},
    .new_end_point={.row=0, .column=4}
  };
  edits[1] = (TSInputEdit) {
    .start_byte=23,
    .old_end_byte=23,
    .new_end_byte=45,
    .start_point={.row=0, .column=23},
    .old_end_point={.row=0, .column=23},
    .new_end_point={.row=0, .column=45}
  };
  test_edit_script(source, dest, 21, 21, edit_count, edits, parser, lit_map);
}

TEST_F(IncrementalExpFixture, swap_updates) {
  const char *source = "Add(Num(2), Num(3))";
  const char *dest = "Add(Num(3), Num(2))";
  const int edit_count = 2;
  TSInputEdit edits[edit_count];
  edits[0] = (TSInputEdit) {
    .start_byte=8,
    .old_end_byte=9,
    .new_end_byte=9,
    .start_point={.row=0, .column=8},
    .old_end_point={.row=0, .column=9},
    .new_end_point={.row=0, .column=9}
  };
  edits[1] = (TSInputEdit) {
    .start_byte=16,
    .old_end_byte=17,
    .new_end_byte=17,
    .start_point={.row=0, .column=16},
    .old_end_point={.row=0, .column=17},
    .new_end_point={.row=0, .column=17}
  };
  test_edit_script(source, dest, 2, 2, edit_count, edits, parser, lit_map);
}

