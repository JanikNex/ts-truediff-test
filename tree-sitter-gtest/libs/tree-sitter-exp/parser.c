#include "tree_sitter/parser.h"

#if defined(__GNUC__) || defined(__clang__)
#pragma GCC diagnostic push
#pragma GCC diagnostic ignored "-Wmissing-field-initializers"
#endif

#define LANGUAGE_VERSION 13
#define STATE_COUNT 43
#define LARGE_STATE_COUNT 10
#define SYMBOL_COUNT 24
#define ALIAS_COUNT 0
#define TOKEN_COUNT 14
#define EXTERNAL_TOKEN_COUNT 0
#define FIELD_COUNT 7
#define MAX_ALIAS_SEQUENCE_LENGTH 8
#define PRODUCTION_ID_COUNT 5

enum {
  anon_sym_Add = 1,
  anon_sym_LPAREN = 2,
  anon_sym_COMMA = 3,
  anon_sym_RPAREN = 4,
  anon_sym_Sub = 5,
  anon_sym_Mul = 6,
  anon_sym_Num = 7,
  anon_sym_Var = 8,
  anon_sym_Let = 9,
  sym_number = 10,
  anon_sym_DQUOTE = 11,
  aux_sym_string_token1 = 12,
  aux_sym_string_token2 = 13,
  sym_source = 14,
  sym__expr = 15,
  sym_add = 16,
  sym_sub = 17,
  sym_mul = 18,
  sym_num = 19,
  sym_var = 20,
  sym_let = 21,
  sym_string = 22,
  aux_sym_string_repeat1 = 23,
};

static const char * const ts_symbol_names[] = {
  [ts_builtin_sym_end] = "end",
  [anon_sym_Add] = "Add",
  [anon_sym_LPAREN] = "(",
  [anon_sym_COMMA] = ",",
  [anon_sym_RPAREN] = ")",
  [anon_sym_Sub] = "Sub",
  [anon_sym_Mul] = "Mul",
  [anon_sym_Num] = "Num",
  [anon_sym_Var] = "Var",
  [anon_sym_Let] = "Let",
  [sym_number] = "number",
  [anon_sym_DQUOTE] = "\"",
  [aux_sym_string_token1] = "string_token1",
  [aux_sym_string_token2] = "string_token2",
  [sym_source] = "source",
  [sym__expr] = "_expr",
  [sym_add] = "add",
  [sym_sub] = "sub",
  [sym_mul] = "mul",
  [sym_num] = "num",
  [sym_var] = "var",
  [sym_let] = "let",
  [sym_string] = "string",
  [aux_sym_string_repeat1] = "string_repeat1",
};

static const TSSymbol ts_symbol_map[] = {
  [ts_builtin_sym_end] = ts_builtin_sym_end,
  [anon_sym_Add] = anon_sym_Add,
  [anon_sym_LPAREN] = anon_sym_LPAREN,
  [anon_sym_COMMA] = anon_sym_COMMA,
  [anon_sym_RPAREN] = anon_sym_RPAREN,
  [anon_sym_Sub] = anon_sym_Sub,
  [anon_sym_Mul] = anon_sym_Mul,
  [anon_sym_Num] = anon_sym_Num,
  [anon_sym_Var] = anon_sym_Var,
  [anon_sym_Let] = anon_sym_Let,
  [sym_number] = sym_number,
  [anon_sym_DQUOTE] = anon_sym_DQUOTE,
  [aux_sym_string_token1] = aux_sym_string_token1,
  [aux_sym_string_token2] = aux_sym_string_token2,
  [sym_source] = sym_source,
  [sym__expr] = sym__expr,
  [sym_add] = sym_add,
  [sym_sub] = sym_sub,
  [sym_mul] = sym_mul,
  [sym_num] = sym_num,
  [sym_var] = sym_var,
  [sym_let] = sym_let,
  [sym_string] = sym_string,
  [aux_sym_string_repeat1] = aux_sym_string_repeat1,
};

static const TSSymbolMetadata ts_symbol_metadata[] = {
  [ts_builtin_sym_end] = {
    .visible = false,
    .named = true,
  },
  [anon_sym_Add] = {
    .visible = true,
    .named = false,
  },
  [anon_sym_LPAREN] = {
    .visible = true,
    .named = false,
  },
  [anon_sym_COMMA] = {
    .visible = true,
    .named = false,
  },
  [anon_sym_RPAREN] = {
    .visible = true,
    .named = false,
  },
  [anon_sym_Sub] = {
    .visible = true,
    .named = false,
  },
  [anon_sym_Mul] = {
    .visible = true,
    .named = false,
  },
  [anon_sym_Num] = {
    .visible = true,
    .named = false,
  },
  [anon_sym_Var] = {
    .visible = true,
    .named = false,
  },
  [anon_sym_Let] = {
    .visible = true,
    .named = false,
  },
  [sym_number] = {
    .visible = true,
    .named = true,
  },
  [anon_sym_DQUOTE] = {
    .visible = true,
    .named = false,
  },
  [aux_sym_string_token1] = {
    .visible = false,
    .named = false,
  },
  [aux_sym_string_token2] = {
    .visible = false,
    .named = false,
  },
  [sym_source] = {
    .visible = true,
    .named = true,
  },
  [sym__expr] = {
    .visible = false,
    .named = true,
  },
  [sym_add] = {
    .visible = true,
    .named = true,
  },
  [sym_sub] = {
    .visible = true,
    .named = true,
  },
  [sym_mul] = {
    .visible = true,
    .named = true,
  },
  [sym_num] = {
    .visible = true,
    .named = true,
  },
  [sym_var] = {
    .visible = true,
    .named = true,
  },
  [sym_let] = {
    .visible = true,
    .named = true,
  },
  [sym_string] = {
    .visible = true,
    .named = true,
  },
  [aux_sym_string_repeat1] = {
    .visible = false,
    .named = false,
  },
};

enum {
  field_body = 1,
  field_e = 2,
  field_e1 = 3,
  field_e2 = 4,
  field_n = 5,
  field_name = 6,
  field_x = 7,
};

static const char * const ts_field_names[] = {
  [0] = NULL,
  [field_body] = "body",
  [field_e] = "e",
  [field_e1] = "e1",
  [field_e2] = "e2",
  [field_n] = "n",
  [field_name] = "name",
  [field_x] = "x",
};

static const TSFieldMapSlice ts_field_map_slices[PRODUCTION_ID_COUNT] = {
  [1] = {.index = 0, .length = 1},
  [2] = {.index = 1, .length = 1},
  [3] = {.index = 2, .length = 2},
  [4] = {.index = 4, .length = 3},
};

static const TSFieldMapEntry ts_field_map_entries[] = {
  [0] =
    {field_n, 2},
  [1] =
    {field_name, 2},
  [2] =
    {field_e1, 2},
    {field_e2, 4},
  [4] =
    {field_body, 6},
    {field_e, 4},
    {field_x, 2},
};

static const TSSymbol ts_alias_sequences[PRODUCTION_ID_COUNT][MAX_ALIAS_SEQUENCE_LENGTH] = {
  [0] = {0},
};

static const uint16_t ts_non_terminal_alias_map[] = {
  0,
};

static bool ts_lex(TSLexer *lexer, TSStateId state) {
  START_LEXER();
  eof = lexer->eof(lexer);
  switch (state) {
    case 0:
      if (eof) ADVANCE(15);
      if (lookahead == '"') ADVANCE(26);
      if (lookahead == '(') ADVANCE(17);
      if (lookahead == ')') ADVANCE(19);
      if (lookahead == ',') ADVANCE(18);
      if (lookahead == 'A') ADVANCE(5);
      if (lookahead == 'L') ADVANCE(6);
      if (lookahead == 'M') ADVANCE(11);
      if (lookahead == 'N') ADVANCE(12);
      if (lookahead == 'S') ADVANCE(13);
      if (lookahead == 'V') ADVANCE(2);
      if (lookahead == '\\') ADVANCE(14);
      if (lookahead == '\t' ||
          lookahead == '\n' ||
          lookahead == '\r' ||
          lookahead == ' ') SKIP(0)
      if (('0' <= lookahead && lookahead <= '9')) ADVANCE(25);
      END_STATE();
    case 1:
      if (lookahead == '\n') SKIP(1)
      if (lookahead == '"') ADVANCE(26);
      if (lookahead == '\\') ADVANCE(14);
      if (lookahead == '\t' ||
          lookahead == '\r' ||
          lookahead == ' ') ADVANCE(28);
      if (lookahead != 0) ADVANCE(27);
      END_STATE();
    case 2:
      if (lookahead == 'a') ADVANCE(9);
      END_STATE();
    case 3:
      if (lookahead == 'b') ADVANCE(20);
      END_STATE();
    case 4:
      if (lookahead == 'd') ADVANCE(16);
      END_STATE();
    case 5:
      if (lookahead == 'd') ADVANCE(4);
      END_STATE();
    case 6:
      if (lookahead == 'e') ADVANCE(10);
      END_STATE();
    case 7:
      if (lookahead == 'l') ADVANCE(21);
      END_STATE();
    case 8:
      if (lookahead == 'm') ADVANCE(22);
      END_STATE();
    case 9:
      if (lookahead == 'r') ADVANCE(23);
      END_STATE();
    case 10:
      if (lookahead == 't') ADVANCE(24);
      END_STATE();
    case 11:
      if (lookahead == 'u') ADVANCE(7);
      END_STATE();
    case 12:
      if (lookahead == 'u') ADVANCE(8);
      END_STATE();
    case 13:
      if (lookahead == 'u') ADVANCE(3);
      END_STATE();
    case 14:
      if (lookahead != 0) ADVANCE(29);
      END_STATE();
    case 15:
      ACCEPT_TOKEN(ts_builtin_sym_end);
      END_STATE();
    case 16:
      ACCEPT_TOKEN(anon_sym_Add);
      END_STATE();
    case 17:
      ACCEPT_TOKEN(anon_sym_LPAREN);
      END_STATE();
    case 18:
      ACCEPT_TOKEN(anon_sym_COMMA);
      END_STATE();
    case 19:
      ACCEPT_TOKEN(anon_sym_RPAREN);
      END_STATE();
    case 20:
      ACCEPT_TOKEN(anon_sym_Sub);
      END_STATE();
    case 21:
      ACCEPT_TOKEN(anon_sym_Mul);
      END_STATE();
    case 22:
      ACCEPT_TOKEN(anon_sym_Num);
      END_STATE();
    case 23:
      ACCEPT_TOKEN(anon_sym_Var);
      END_STATE();
    case 24:
      ACCEPT_TOKEN(anon_sym_Let);
      END_STATE();
    case 25:
      ACCEPT_TOKEN(sym_number);
      if (('0' <= lookahead && lookahead <= '9')) ADVANCE(25);
      END_STATE();
    case 26:
      ACCEPT_TOKEN(anon_sym_DQUOTE);
      END_STATE();
    case 27:
      ACCEPT_TOKEN(aux_sym_string_token1);
      END_STATE();
    case 28:
      ACCEPT_TOKEN(aux_sym_string_token1);
      if (lookahead == '\t' ||
          lookahead == '\r' ||
          lookahead == ' ') ADVANCE(28);
      if (lookahead != 0 &&
          lookahead != '\n' &&
          lookahead != '"' &&
          lookahead != '\\') ADVANCE(27);
      END_STATE();
    case 29:
      ACCEPT_TOKEN(aux_sym_string_token2);
      END_STATE();
    default:
      return false;
  }
}

static const TSLexMode ts_lex_modes[STATE_COUNT] = {
  [0] = {.lex_state = 0},
  [1] = {.lex_state = 0},
  [2] = {.lex_state = 0},
  [3] = {.lex_state = 0},
  [4] = {.lex_state = 0},
  [5] = {.lex_state = 0},
  [6] = {.lex_state = 0},
  [7] = {.lex_state = 0},
  [8] = {.lex_state = 0},
  [9] = {.lex_state = 0},
  [10] = {.lex_state = 1},
  [11] = {.lex_state = 1},
  [12] = {.lex_state = 1},
  [13] = {.lex_state = 0},
  [14] = {.lex_state = 0},
  [15] = {.lex_state = 0},
  [16] = {.lex_state = 0},
  [17] = {.lex_state = 0},
  [18] = {.lex_state = 0},
  [19] = {.lex_state = 0},
  [20] = {.lex_state = 0},
  [21] = {.lex_state = 0},
  [22] = {.lex_state = 0},
  [23] = {.lex_state = 0},
  [24] = {.lex_state = 0},
  [25] = {.lex_state = 0},
  [26] = {.lex_state = 0},
  [27] = {.lex_state = 0},
  [28] = {.lex_state = 0},
  [29] = {.lex_state = 0},
  [30] = {.lex_state = 0},
  [31] = {.lex_state = 0},
  [32] = {.lex_state = 0},
  [33] = {.lex_state = 0},
  [34] = {.lex_state = 0},
  [35] = {.lex_state = 0},
  [36] = {.lex_state = 0},
  [37] = {.lex_state = 0},
  [38] = {.lex_state = 0},
  [39] = {.lex_state = 0},
  [40] = {.lex_state = 0},
  [41] = {.lex_state = 0},
  [42] = {.lex_state = 0},
};

static const uint16_t ts_parse_table[LARGE_STATE_COUNT][SYMBOL_COUNT] = {
  [0] = {
    [ts_builtin_sym_end] = ACTIONS(1),
    [anon_sym_Add] = ACTIONS(1),
    [anon_sym_LPAREN] = ACTIONS(1),
    [anon_sym_COMMA] = ACTIONS(1),
    [anon_sym_RPAREN] = ACTIONS(1),
    [anon_sym_Sub] = ACTIONS(1),
    [anon_sym_Mul] = ACTIONS(1),
    [anon_sym_Num] = ACTIONS(1),
    [anon_sym_Var] = ACTIONS(1),
    [anon_sym_Let] = ACTIONS(1),
    [sym_number] = ACTIONS(1),
    [anon_sym_DQUOTE] = ACTIONS(1),
    [aux_sym_string_token2] = ACTIONS(1),
  },
  [1] = {
    [sym_source] = STATE(38),
    [sym__expr] = STATE(26),
    [sym_add] = STATE(26),
    [sym_sub] = STATE(26),
    [sym_mul] = STATE(26),
    [sym_num] = STATE(26),
    [sym_var] = STATE(26),
    [sym_let] = STATE(26),
    [ts_builtin_sym_end] = ACTIONS(3),
    [anon_sym_Add] = ACTIONS(5),
    [anon_sym_Sub] = ACTIONS(7),
    [anon_sym_Mul] = ACTIONS(9),
    [anon_sym_Num] = ACTIONS(11),
    [anon_sym_Var] = ACTIONS(13),
    [anon_sym_Let] = ACTIONS(15),
  },
  [2] = {
    [sym__expr] = STATE(36),
    [sym_add] = STATE(36),
    [sym_sub] = STATE(36),
    [sym_mul] = STATE(36),
    [sym_num] = STATE(36),
    [sym_var] = STATE(36),
    [sym_let] = STATE(36),
    [anon_sym_Add] = ACTIONS(5),
    [anon_sym_Sub] = ACTIONS(7),
    [anon_sym_Mul] = ACTIONS(9),
    [anon_sym_Num] = ACTIONS(11),
    [anon_sym_Var] = ACTIONS(13),
    [anon_sym_Let] = ACTIONS(15),
  },
  [3] = {
    [sym__expr] = STATE(32),
    [sym_add] = STATE(32),
    [sym_sub] = STATE(32),
    [sym_mul] = STATE(32),
    [sym_num] = STATE(32),
    [sym_var] = STATE(32),
    [sym_let] = STATE(32),
    [anon_sym_Add] = ACTIONS(5),
    [anon_sym_Sub] = ACTIONS(7),
    [anon_sym_Mul] = ACTIONS(9),
    [anon_sym_Num] = ACTIONS(11),
    [anon_sym_Var] = ACTIONS(13),
    [anon_sym_Let] = ACTIONS(15),
  },
  [4] = {
    [sym__expr] = STATE(41),
    [sym_add] = STATE(41),
    [sym_sub] = STATE(41),
    [sym_mul] = STATE(41),
    [sym_num] = STATE(41),
    [sym_var] = STATE(41),
    [sym_let] = STATE(41),
    [anon_sym_Add] = ACTIONS(5),
    [anon_sym_Sub] = ACTIONS(7),
    [anon_sym_Mul] = ACTIONS(9),
    [anon_sym_Num] = ACTIONS(11),
    [anon_sym_Var] = ACTIONS(13),
    [anon_sym_Let] = ACTIONS(15),
  },
  [5] = {
    [sym__expr] = STATE(31),
    [sym_add] = STATE(31),
    [sym_sub] = STATE(31),
    [sym_mul] = STATE(31),
    [sym_num] = STATE(31),
    [sym_var] = STATE(31),
    [sym_let] = STATE(31),
    [anon_sym_Add] = ACTIONS(5),
    [anon_sym_Sub] = ACTIONS(7),
    [anon_sym_Mul] = ACTIONS(9),
    [anon_sym_Num] = ACTIONS(11),
    [anon_sym_Var] = ACTIONS(13),
    [anon_sym_Let] = ACTIONS(15),
  },
  [6] = {
    [sym__expr] = STATE(35),
    [sym_add] = STATE(35),
    [sym_sub] = STATE(35),
    [sym_mul] = STATE(35),
    [sym_num] = STATE(35),
    [sym_var] = STATE(35),
    [sym_let] = STATE(35),
    [anon_sym_Add] = ACTIONS(5),
    [anon_sym_Sub] = ACTIONS(7),
    [anon_sym_Mul] = ACTIONS(9),
    [anon_sym_Num] = ACTIONS(11),
    [anon_sym_Var] = ACTIONS(13),
    [anon_sym_Let] = ACTIONS(15),
  },
  [7] = {
    [sym__expr] = STATE(34),
    [sym_add] = STATE(34),
    [sym_sub] = STATE(34),
    [sym_mul] = STATE(34),
    [sym_num] = STATE(34),
    [sym_var] = STATE(34),
    [sym_let] = STATE(34),
    [anon_sym_Add] = ACTIONS(5),
    [anon_sym_Sub] = ACTIONS(7),
    [anon_sym_Mul] = ACTIONS(9),
    [anon_sym_Num] = ACTIONS(11),
    [anon_sym_Var] = ACTIONS(13),
    [anon_sym_Let] = ACTIONS(15),
  },
  [8] = {
    [sym__expr] = STATE(29),
    [sym_add] = STATE(29),
    [sym_sub] = STATE(29),
    [sym_mul] = STATE(29),
    [sym_num] = STATE(29),
    [sym_var] = STATE(29),
    [sym_let] = STATE(29),
    [anon_sym_Add] = ACTIONS(5),
    [anon_sym_Sub] = ACTIONS(7),
    [anon_sym_Mul] = ACTIONS(9),
    [anon_sym_Num] = ACTIONS(11),
    [anon_sym_Var] = ACTIONS(13),
    [anon_sym_Let] = ACTIONS(15),
  },
  [9] = {
    [sym__expr] = STATE(33),
    [sym_add] = STATE(33),
    [sym_sub] = STATE(33),
    [sym_mul] = STATE(33),
    [sym_num] = STATE(33),
    [sym_var] = STATE(33),
    [sym_let] = STATE(33),
    [anon_sym_Add] = ACTIONS(5),
    [anon_sym_Sub] = ACTIONS(7),
    [anon_sym_Mul] = ACTIONS(9),
    [anon_sym_Num] = ACTIONS(11),
    [anon_sym_Var] = ACTIONS(13),
    [anon_sym_Let] = ACTIONS(15),
  },
};

static const uint16_t ts_small_parse_table[] = {
  [0] = 4,
    ACTIONS(17), 1,
      anon_sym_DQUOTE,
    ACTIONS(19), 1,
      aux_sym_string_token1,
    ACTIONS(22), 1,
      aux_sym_string_token2,
    STATE(10), 1,
      aux_sym_string_repeat1,
  [13] = 4,
    ACTIONS(25), 1,
      anon_sym_DQUOTE,
    ACTIONS(27), 1,
      aux_sym_string_token1,
    ACTIONS(29), 1,
      aux_sym_string_token2,
    STATE(10), 1,
      aux_sym_string_repeat1,
  [26] = 4,
    ACTIONS(31), 1,
      anon_sym_DQUOTE,
    ACTIONS(33), 1,
      aux_sym_string_token1,
    ACTIONS(35), 1,
      aux_sym_string_token2,
    STATE(11), 1,
      aux_sym_string_repeat1,
  [39] = 1,
    ACTIONS(37), 3,
      ts_builtin_sym_end,
      anon_sym_COMMA,
      anon_sym_RPAREN,
  [45] = 1,
    ACTIONS(39), 3,
      ts_builtin_sym_end,
      anon_sym_COMMA,
      anon_sym_RPAREN,
  [51] = 1,
    ACTIONS(41), 3,
      ts_builtin_sym_end,
      anon_sym_COMMA,
      anon_sym_RPAREN,
  [57] = 1,
    ACTIONS(43), 3,
      ts_builtin_sym_end,
      anon_sym_COMMA,
      anon_sym_RPAREN,
  [63] = 1,
    ACTIONS(45), 3,
      ts_builtin_sym_end,
      anon_sym_COMMA,
      anon_sym_RPAREN,
  [69] = 1,
    ACTIONS(47), 3,
      ts_builtin_sym_end,
      anon_sym_COMMA,
      anon_sym_RPAREN,
  [75] = 2,
    ACTIONS(49), 1,
      anon_sym_DQUOTE,
    STATE(27), 1,
      sym_string,
  [82] = 2,
    ACTIONS(49), 1,
      anon_sym_DQUOTE,
    STATE(23), 1,
      sym_string,
  [89] = 1,
    ACTIONS(51), 2,
      anon_sym_COMMA,
      anon_sym_RPAREN,
  [94] = 1,
    ACTIONS(53), 2,
      anon_sym_COMMA,
      anon_sym_RPAREN,
  [99] = 1,
    ACTIONS(55), 1,
      anon_sym_COMMA,
  [103] = 1,
    ACTIONS(57), 1,
      anon_sym_LPAREN,
  [107] = 1,
    ACTIONS(59), 1,
      anon_sym_LPAREN,
  [111] = 1,
    ACTIONS(61), 1,
      ts_builtin_sym_end,
  [115] = 1,
    ACTIONS(63), 1,
      anon_sym_RPAREN,
  [119] = 1,
    ACTIONS(65), 1,
      anon_sym_RPAREN,
  [123] = 1,
    ACTIONS(67), 1,
      anon_sym_COMMA,
  [127] = 1,
    ACTIONS(69), 1,
      anon_sym_LPAREN,
  [131] = 1,
    ACTIONS(71), 1,
      anon_sym_RPAREN,
  [135] = 1,
    ACTIONS(73), 1,
      anon_sym_RPAREN,
  [139] = 1,
    ACTIONS(75), 1,
      anon_sym_RPAREN,
  [143] = 1,
    ACTIONS(77), 1,
      anon_sym_COMMA,
  [147] = 1,
    ACTIONS(79), 1,
      anon_sym_COMMA,
  [151] = 1,
    ACTIONS(81), 1,
      anon_sym_COMMA,
  [155] = 1,
    ACTIONS(83), 1,
      sym_number,
  [159] = 1,
    ACTIONS(85), 1,
      ts_builtin_sym_end,
  [163] = 1,
    ACTIONS(87), 1,
      anon_sym_LPAREN,
  [167] = 1,
    ACTIONS(89), 1,
      anon_sym_LPAREN,
  [171] = 1,
    ACTIONS(91), 1,
      anon_sym_RPAREN,
  [175] = 1,
    ACTIONS(93), 1,
      anon_sym_LPAREN,
};

static const uint32_t ts_small_parse_table_map[] = {
  [SMALL_STATE(10)] = 0,
  [SMALL_STATE(11)] = 13,
  [SMALL_STATE(12)] = 26,
  [SMALL_STATE(13)] = 39,
  [SMALL_STATE(14)] = 45,
  [SMALL_STATE(15)] = 51,
  [SMALL_STATE(16)] = 57,
  [SMALL_STATE(17)] = 63,
  [SMALL_STATE(18)] = 69,
  [SMALL_STATE(19)] = 75,
  [SMALL_STATE(20)] = 82,
  [SMALL_STATE(21)] = 89,
  [SMALL_STATE(22)] = 94,
  [SMALL_STATE(23)] = 99,
  [SMALL_STATE(24)] = 103,
  [SMALL_STATE(25)] = 107,
  [SMALL_STATE(26)] = 111,
  [SMALL_STATE(27)] = 115,
  [SMALL_STATE(28)] = 119,
  [SMALL_STATE(29)] = 123,
  [SMALL_STATE(30)] = 127,
  [SMALL_STATE(31)] = 131,
  [SMALL_STATE(32)] = 135,
  [SMALL_STATE(33)] = 139,
  [SMALL_STATE(34)] = 143,
  [SMALL_STATE(35)] = 147,
  [SMALL_STATE(36)] = 151,
  [SMALL_STATE(37)] = 155,
  [SMALL_STATE(38)] = 159,
  [SMALL_STATE(39)] = 163,
  [SMALL_STATE(40)] = 167,
  [SMALL_STATE(41)] = 171,
  [SMALL_STATE(42)] = 175,
};

static const TSParseActionEntry ts_parse_actions[] = {
  [0] = {.entry = {.count = 0, .reusable = false}},
  [1] = {.entry = {.count = 1, .reusable = false}}, RECOVER(),
  [3] = {.entry = {.count = 1, .reusable = true}}, REDUCE(sym_source, 0),
  [5] = {.entry = {.count = 1, .reusable = true}}, SHIFT(30),
  [7] = {.entry = {.count = 1, .reusable = true}}, SHIFT(24),
  [9] = {.entry = {.count = 1, .reusable = true}}, SHIFT(25),
  [11] = {.entry = {.count = 1, .reusable = true}}, SHIFT(42),
  [13] = {.entry = {.count = 1, .reusable = true}}, SHIFT(40),
  [15] = {.entry = {.count = 1, .reusable = true}}, SHIFT(39),
  [17] = {.entry = {.count = 1, .reusable = false}}, REDUCE(aux_sym_string_repeat1, 2),
  [19] = {.entry = {.count = 2, .reusable = true}}, REDUCE(aux_sym_string_repeat1, 2), SHIFT_REPEAT(10),
  [22] = {.entry = {.count = 2, .reusable = false}}, REDUCE(aux_sym_string_repeat1, 2), SHIFT_REPEAT(10),
  [25] = {.entry = {.count = 1, .reusable = false}}, SHIFT(21),
  [27] = {.entry = {.count = 1, .reusable = true}}, SHIFT(10),
  [29] = {.entry = {.count = 1, .reusable = false}}, SHIFT(10),
  [31] = {.entry = {.count = 1, .reusable = false}}, SHIFT(22),
  [33] = {.entry = {.count = 1, .reusable = true}}, SHIFT(11),
  [35] = {.entry = {.count = 1, .reusable = false}}, SHIFT(11),
  [37] = {.entry = {.count = 1, .reusable = true}}, REDUCE(sym_let, 8, .production_id = 4),
  [39] = {.entry = {.count = 1, .reusable = true}}, REDUCE(sym_mul, 6, .production_id = 3),
  [41] = {.entry = {.count = 1, .reusable = true}}, REDUCE(sym_sub, 6, .production_id = 3),
  [43] = {.entry = {.count = 1, .reusable = true}}, REDUCE(sym_add, 6, .production_id = 3),
  [45] = {.entry = {.count = 1, .reusable = true}}, REDUCE(sym_var, 4, .production_id = 2),
  [47] = {.entry = {.count = 1, .reusable = true}}, REDUCE(sym_num, 4, .production_id = 1),
  [49] = {.entry = {.count = 1, .reusable = true}}, SHIFT(12),
  [51] = {.entry = {.count = 1, .reusable = true}}, REDUCE(sym_string, 3),
  [53] = {.entry = {.count = 1, .reusable = true}}, REDUCE(sym_string, 2),
  [55] = {.entry = {.count = 1, .reusable = true}}, SHIFT(2),
  [57] = {.entry = {.count = 1, .reusable = true}}, SHIFT(7),
  [59] = {.entry = {.count = 1, .reusable = true}}, SHIFT(8),
  [61] = {.entry = {.count = 1, .reusable = true}}, REDUCE(sym_source, 1),
  [63] = {.entry = {.count = 1, .reusable = true}}, SHIFT(17),
  [65] = {.entry = {.count = 1, .reusable = true}}, SHIFT(18),
  [67] = {.entry = {.count = 1, .reusable = true}}, SHIFT(9),
  [69] = {.entry = {.count = 1, .reusable = true}}, SHIFT(6),
  [71] = {.entry = {.count = 1, .reusable = true}}, SHIFT(16),
  [73] = {.entry = {.count = 1, .reusable = true}}, SHIFT(15),
  [75] = {.entry = {.count = 1, .reusable = true}}, SHIFT(14),
  [77] = {.entry = {.count = 1, .reusable = true}}, SHIFT(3),
  [79] = {.entry = {.count = 1, .reusable = true}}, SHIFT(5),
  [81] = {.entry = {.count = 1, .reusable = true}}, SHIFT(4),
  [83] = {.entry = {.count = 1, .reusable = true}}, SHIFT(28),
  [85] = {.entry = {.count = 1, .reusable = true}},  ACCEPT_INPUT(),
  [87] = {.entry = {.count = 1, .reusable = true}}, SHIFT(20),
  [89] = {.entry = {.count = 1, .reusable = true}}, SHIFT(19),
  [91] = {.entry = {.count = 1, .reusable = true}}, SHIFT(13),
  [93] = {.entry = {.count = 1, .reusable = true}}, SHIFT(37),
};

#ifdef __cplusplus
extern "C" {
#endif
#ifdef _WIN32
#define extern __declspec(dllexport)
#endif

extern const TSLanguage *tree_sitter_exp(void) {
  static const TSLanguage language = {
    .version = LANGUAGE_VERSION,
    .symbol_count = SYMBOL_COUNT,
    .alias_count = ALIAS_COUNT,
    .token_count = TOKEN_COUNT,
    .external_token_count = EXTERNAL_TOKEN_COUNT,
    .state_count = STATE_COUNT,
    .large_state_count = LARGE_STATE_COUNT,
    .production_id_count = PRODUCTION_ID_COUNT,
    .field_count = FIELD_COUNT,
    .max_alias_sequence_length = MAX_ALIAS_SEQUENCE_LENGTH,
    .parse_table = &ts_parse_table[0][0],
    .small_parse_table = ts_small_parse_table,
    .small_parse_table_map = ts_small_parse_table_map,
    .parse_actions = ts_parse_actions,
    .symbol_names = ts_symbol_names,
    .field_names = ts_field_names,
    .field_map_slices = ts_field_map_slices,
    .field_map_entries = ts_field_map_entries,
    .symbol_metadata = ts_symbol_metadata,
    .public_symbol_map = ts_symbol_map,
    .alias_map = ts_non_terminal_alias_map,
    .alias_sequences = &ts_alias_sequences[0][0],
    .lex_modes = ts_lex_modes,
    .lex_fn = ts_lex,
  };
  return &language;
}
#ifdef __cplusplus
}
#endif
