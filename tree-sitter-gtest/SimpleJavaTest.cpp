#include <cstring>
#include "libs/tree-sitter/includes/parser.h"
#include "libs/tree-sitter/includes/api.h"
#include "libs/debug.h"
#include "gtest/gtest.h"

extern "C" TSLanguage *tree_sitter_java();

static const TSLanguage *lang = tree_sitter_java();

class SimpleJavaFixture : public ::testing::Test {
public:
  SimpleJavaFixture() {
    // Create a parser.
    parser = ts_parser_new();

    // Set the parser's language.
    ts_parser_set_language(parser, lang);

    lit_map = ts_literal_map_create(lang);
    ts_literal_map_add_literal(lit_map, 1);   // identifier
    ts_literal_map_add_literal(lit_map, 2);   // decimal_integer_literal
    ts_literal_map_add_literal(lit_map, 3);   // hex_integer_literal
    ts_literal_map_add_literal(lit_map, 4);   // octal_integer_literal
    ts_literal_map_add_literal(lit_map, 5);   // binary_integer_literal
    ts_literal_map_add_literal(lit_map, 6);   // decimal_floating_point_literal
    ts_literal_map_add_literal(lit_map, 7);   // hex_floating_point_literal
    ts_literal_map_add_literal(lit_map, 10);  // character_literal
    ts_literal_map_add_literal(lit_map, 11);  // string_literal
    ts_literal_map_add_literal(lit_map, 12);  // null_literal
    ts_literal_map_add_literal(lit_map, 122); // comment
  }

protected:
  ~SimpleJavaFixture() override {
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
  print_edit_script(tree->language,diff_result.edit_script);

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


TEST_F(SimpleJavaFixture, no_change) {
  const char *source = "public class Main {\n"
                       "  int x = 5;\n"
                       "}";
  const char *dest = "public class Main {\n"
                     "  int x = 5;\n"
                     "}";
  test_edit_script(source, dest, 0, parser, lit_map);
}

TEST_F(SimpleJavaFixture, updated_integer) {
  const char *source = "public class Main {\n"
                       "  int x = 5;\n"
                       "}";
  const char *dest = "public class Main {\n"
                     "  int x = 6;\n"
                     "}";
  test_edit_script(source, dest, 1, parser, lit_map);
}

TEST_F(SimpleJavaFixture, updated_variable_name) {
  const char *source = "public class Main {\n"
                       "  int x = 5;\n"
                       "}";
  const char *dest = "public class Main {\n"
                     "  int y = 5;\n"
                     "}";
  test_edit_script(source, dest, 1, parser, lit_map);
}

TEST_F(SimpleJavaFixture, updated_identifier_and_integer) {
  const char *source = "public class Main {\n"
                       "  int x = 5;\n"
                       "}";
  const char *dest = "public class Main {\n"
                     "  int y = 6;\n"
                     "}";
  test_edit_script(source, dest, 2, parser, lit_map);
}

TEST_F(SimpleJavaFixture, updated_multiple_identifiers_and_integer) {
  const char *source = "public class Main {\n"
                       "  int x = 5;\n"
                       "}";
  const char *dest = "public class MainClass {\n"
                     "  int y = 6;\n"
                     "}";
  test_edit_script(source, dest, 3, parser, lit_map);
}

TEST_F(SimpleJavaFixture, added_new_free_line) {
  const char *source = "public class Main {\n"
                       "  int x = 5;\n"
                       "}";
  const char *dest = "public class Main {\n"
                     "\n"
                     "  int x = 5;\n"
                     "}";
  test_edit_script(source, dest, 4, parser, lit_map);
}

TEST_F(SimpleJavaFixture, added_new_modifier) {
  const char *source = "class Main {\n"
                       "  int x = 5;\n"
                       "}";
  const char *dest = "public class Main {\n"
                     "  int x = 5;\n"
                     "}";
  test_edit_script(source, dest, 5, parser, lit_map);
}

TEST_F(SimpleJavaFixture, added_modifier) {
  const char *source = "public class Main {\n"
                       "  int x = 5;\n"
                       "}";
  const char *dest = "public final class Main {\n"
                     "  int x = 5;\n"
                     "}";
  test_edit_script(source, dest, 3, parser, lit_map);
}

TEST_F(SimpleJavaFixture, added_function) {
  const char *source = "public class Main {\n"
                       "}";
  const char *dest = "public class Main {\n"
                     "  void myMethod() {\n"
                     "      System.out.println(\"I just got executed!\");\n"
                     "  }\n"
                     "}";
  test_edit_script(source, dest, 35, parser, lit_map);
}

TEST_F(SimpleJavaFixture, added_new_function_modifier) {
  const char *source = "public class Main {\n"
                       "  void myMethod() {\n"
                       "      System.out.println(\"I just got executed!\");\n"
                       "  }\n"
                       "}";
  const char *dest = "public class Main {\n"
                     "  public void myMethod() {\n"
                     "      System.out.println(\"I just got executed!\");\n"
                     "  }\n"
                     "}";
  test_edit_script(source, dest, 7, parser, lit_map);
}

TEST_F(SimpleJavaFixture, added_function_modifier) {
  const char *source = "public class Main {\n"
                       "  public void myMethod() {\n"
                       "      System.out.println(\"I just got executed!\");\n"
                       "  }\n"
                       "}";
  const char *dest = "public class Main {\n"
                     "  public static void myMethod() {\n"
                     "      System.out.println(\"I just got executed!\");\n"
                     "  }\n"
                     "}";
  test_edit_script(source, dest, 3, parser, lit_map);
}

TEST_F(SimpleJavaFixture, updated_string) {
  const char *source = "public class Main {\n"
                       "  public void myMethod() {\n"
                       "      System.out.println(\"I just got executed!\");\n"
                       "  }\n"
                       "}";
  const char *dest = "public class Main {\n"
                     "  public void myMethod() {\n"
                     "      System.out.println(\"I absolutely got executed!\");\n"
                     "  }\n"
                     "}";
  test_edit_script(source, dest, 1, parser, lit_map);
}

TEST_F(SimpleJavaFixture, swap_function_order_one) {
  const char *source = "public class Main {\n"
                       "  void myMethod() {\n"
                       "      return false;\n"
                       "  }\n"
                       "  void myOtherMethod() {\n"
                       "      return true;\n"
                       "  }\n"
                       "}";
  const char *dest = "public class Main {\n"
                     "  void myOtherMethod() {\n"
                     "      return true;\n"
                     "  }\n"
                     "  void myMethod() {\n"
                     "      return false;\n"
                     "  }\n"
                     "}";
  test_edit_script(source, dest, 4, parser, lit_map);
}

TEST_F(SimpleJavaFixture, invert_for_loop) {
  const char *source = "public class Main {\n"
                       "  void myMethod() {\n"
                       "    for (int i = 0; i < 5; i++) {\n"
                       "      System.out.println(i);\n"
                       "    }\n"
                       "  }\n"
                       "  void myOtherMethod() {\n"
                       "      return true;\n"
                       "  }\n"
                       "}";
  const char *dest = "public class Main {\n"
                     "  void myMethod() {\n"
                     "    for (int i = 4; i >= 0; i--) {\n"
                     "      System.out.println(i);\n"
                     "    }\n"
                     "  }\n"
                     "  void myOtherMethod() {\n"
                     "      return true;\n"
                     "  }\n"
                     "}";
  test_edit_script(source, dest, 6, parser, lit_map);
}

TEST_F(SimpleJavaFixture, swap_loops) {
  const char *source = "public class Main {\n"
                       "  void myMethod() {\n"
                       "    for (int i = 0; i < 5; i++) {\n"
                       "      System.out.println(i);\n"
                       "    }\n"
                       "  }\n"
                       "  void myOtherMethod() {\n"
                       "    for (int i = 4; i >= 0; i--) {\n"
                       "      System.out.println(i);\n"
                       "    }\n"
                       "  }\n"
                       "}";
  const char *dest = "public class Main {\n"
                     "  void myMethod() {\n"
                     "    for (int i = 4; i >= 0; i--) {\n"
                     "      System.out.println(i);\n"
                     "    }\n"
                     "  }\n"
                     "  void myOtherMethod() {\n"
                     "    for (int i = 0; i < 5; i++) {\n"
                     "      System.out.println(i);\n"
                     "    }\n"
                     "  }\n"
                     "}";
  test_edit_script(source, dest, 6, parser, lit_map);
}

