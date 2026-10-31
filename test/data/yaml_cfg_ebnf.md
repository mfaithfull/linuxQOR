# YAML 1.2.2 — Context-Free (CFG / EBNF) Encoding

This is a **context-free (CFG) encoding** of the YAML 1.2.2 grammar in EBNF.
It is a faithful rewrite of `yaml_grammar.md` (the spec's parameterized
grammar) in which all context-sensitivity has been removed:

* the grammar-context parameter **`c`** ∈ {`BLOCK-OUT`, `BLOCK-IN`,
  `FLOW-OUT`, `FLOW-IN`, `BLOCK-KEY`, `FLOW-KEY`} and the chomping parameter
  **`t`** ∈ {`STRIP`, `CLIP`, `KEEP`} are **expanded into named nonterminals**
  (the context becomes part of the name, so no rule carries a live context);
* the integer indentation parameter **`n`** (and the offset `m`) is supplied by
  the **lexer**, which emits an `INDENT(i)` / `DEDENT` token stream
  (the standard indentation mechanism; the CFG itself is then context-free);
* the two **lookahead / lookbehind** tests (`[ lookahead … ]`,
  `[ lookbehind … ]`) are folded into the character-class definitions so no
  rule needs a back/forward reference.

Character-set differences are expressed with the explicit `-` operator on
named character classes (EBNF's `set-difference`, as used by the spec itself);
these are pure lexical (regular) definitions and do not reintroduce
context-sensitivity.

Notation: `::=` definition, `|` alternation, `?` optional, `*` zero-or-more,
`+` one-or-more, `()` grouping, `""` empty string. Terminal literals in
single quotes; lexical character classes in named nonterminals.
---

## How this is made context-free

The spec's grammar is *parameterized* by three things. Two are
context-sensitivity that we eliminate; one is an integer parameter that is
delegated to the lexer:

1. **Context parameter `c`** (∈ {`BLOCK-OUT`, `BLOCK-IN`, `FLOW-OUT`, `FLOW-IN`,
   `BLOCK-KEY`, `FLOW-KEY`}) and **chomping parameter `t`** (∈ {`STRIP`, `CLIP`,
   `KEEP`}). These are *finite*. A parameterized grammar with a finite
   parameter set is equivalent to a context-free one via finite unfolding: we
   replace every `X(c)` / `Y(t)` by a distinct nonterminal `X_C` / `Y_T` and
   write each macro table as a family of concrete rules. This document does
   that unfolding; no rule carries a live context.

2. **Integer indentation parameter `n`** (and offset `m`). Indentation depth is
   *unbounded*, so it is **not** unfolded in the grammar. Instead the lexer
   performs the indentation scan and emits an explicit `INDENT(k)` token
   (k = depth). Each `s-indent(n)` rule becomes "match an `INDENT` token of
   the column the scanner recorded" — the *scanner* decides `n`, the
   **grammar** does not, so the grammar stays context-free.

3. **Lookahead / lookbehind assertions** (`[ lookahead … ]`, `[ lookbehind … ]`,
   "not followed by non-ws char", "excluding `c-forbidden`"). These are folded
   into the lexical character-class definitions (a CFG/EBNF has no back-
   references, so the constraint is baked into the character set / token).

Naming: a specialized nonterminal is the base name with `_CTX` / `_T` suffixes.
`INDENT(k)` is the indentation token; `INDENT(n)` means a single space at the
scanned column (see §5). `<empty>`/`""` = empty, `<start-of-line>`/`<end-of-input>`
zero-width tokens.
## §5 Character Productions — Context-Free Encoding

These productions are **lexical** and therefore regular. They are already context-free once finite repetitions (`{2}`, `{4}`, `{8}`) are expanded into explicit concatenation.

```ebnf
c_printable ::=
    x09
  | x0A
  | x0D
  | [x20 - x7E]
  | x85
  | [xA0 - xD7FF]
  | [xE000 - xFFFD]
  | [x010000 - x10FFFF]

nb_json ::=
    x09
  | [x20 - x10FFFF]

c_byte_order_mark ::= xFEFF

c_sequence_entry ::= '-'
c_mapping_key      ::= '?'
c_mapping_value    ::= ':'
c_collect_entry    ::= ','
c_sequence_start   ::= '['
c_sequence_end     ::= ']'
c_mapping_start    ::= '{'
c_mapping_end      ::= '}'
c_comment          ::= '#'
c_anchor           ::= '&'
c_alias            ::= '*'
c_tag              ::= '!'
c_literal          ::= '|'
c_folded           ::= '>'
c_single_quote     ::= "'"
c_double_quote     ::= '"'
c_directive        ::= '%'
c_reserved         ::= '@' | '`'

c_indicator ::=
    c_sequence_entry
  | c_mapping_key
  | c_mapping_value
  | c_collect_entry
  | c_sequence_start
  | c_sequence_end
  | c_mapping_start
  | c_mapping_end
  | c_comment
  | c_anchor
  | c_alias
  | c_tag
  | c_literal
  | c_folded
  | c_single_quote
  | c_double_quote
  | c_directive
  | c_reserved

c_flow_indicator ::=
    c_collect_entry
  | c_sequence_start
  | c_sequence_end
  | c_mapping_start
  | c_mapping_end

b_line_feed       ::= x0A
b_carriage_return ::= x0D
b_char ::= b_line_feed | b_carriage_return

nb_char ::= c_printable - b_char - c_byte_order_mark

b_break ::=
    (b_carriage_return b_line_feed)
  | b_carriage_return
  | b_line_feed

b_as_line_feed ::= b_break
b_non_content    ::= b_break

s_space ::= x20
s_tab   ::= x09
s_white ::= s_space | s_tab

ns_char ::= nb_char - s_white

ns_dec_digit ::= [x30 - x39]

ns_hex_digit ::=
    ns_dec_digit
  | [x41 - x46]
  | [x61 - x66]

ns_ascii_letter ::=
    [x41 - x5A]
  | [x61 - x7A]

ns_word_char ::=
    ns_dec_digit
  | ns_ascii_letter
  | '-'

ns_uri_char ::=
    ( '%' ns_hex_digit ns_hex_digit )
  | ns_word_char
  | '#' | ';' | '/' | '?' | ':' | '@' | '&' | '=' | '+'
  | '$' | ',' | '_' | '.' | '!' | '~' | '*' | "'"
  | '(' | ')' | '[' | ']'

ns_tag_char ::= ns_uri_char - c_tag - c_flow_indicator

c_escape ::= '\'

ns_esc_null                ::= '0'
ns_esc_bell                ::= 'a'
ns_esc_backspace           ::= 'b'
ns_esc_horizontal_tab      ::= 't' | x09
ns_esc_line_feed           ::= 'n'
ns_esc_vertical_tab        ::= 'v'
ns_esc_form_feed           ::= 'f'
ns_esc_carriage_return     ::= 'r'
ns_esc_escape              ::= 'e'
ns_esc_space               ::= x20
ns_esc_double_quote        ::= '"'
ns_esc_slash               ::= '/'
ns_esc_backslash           ::= '\'
ns_esc_next_line           ::= 'N'
ns_esc_non_breaking_space  ::= '_'
ns_esc_line_separator      ::= 'L'
ns_esc_paragraph_separator ::= 'P'

ns_esc_8_bit  ::= 'x' ns_hex_digit ns_hex_digit
ns_esc_16_bit ::= 'u' ns_hex_digit ns_hex_digit ns_hex_digit ns_hex_digit
ns_esc_32_bit ::= 'U' ns_hex_digit ns_hex_digit ns_hex_digit ns_hex_digit
                  ns_hex_digit ns_hex_digit ns_hex_digit ns_hex_digit

c_ns_esc_char ::=
  c_escape
  (
      ns_esc_null
    | ns_esc_bell
    | ns_esc_backspace
    | ns_esc_horizontal_tab
    | ns_esc_line_feed
    | ns_esc_vertical_tab
    | ns_esc_form_feed
    | ns_esc_carriage_return
    | ns_esc_escape
    | ns_esc_space
    | ns_esc_double_quote
    | ns_esc_slash
    | ns_esc_backslash
    | ns_esc_next_line
    | ns_esc_non_breaking_space
    | ns_esc_line_separator
    | ns_esc_paragraph_separator
    | ns_esc_8_bit
    | ns_esc_16_bit
    | ns_esc_32_bit
  )
```

## §6 Structural Productions — Context-Free Encoding

The finite context parameter `c` and indentation parameter `n` are unfolded.

### Indentation (lexer-supplied)

Indentation is handled by the lexer emitting `INDENT(i)` tokens (where `i` is
the column recorded by the scanner). The grammar references the token rather
than a parameterized `s-indent(n)`:

```ebnf
INDENT ::= # lexer token: a run of spaces at the scanned column; see §4.2
DEDENT ::= # lexer token: decrease of indent level

# Zero-width anchors and scanner predicates (all match the empty string; the
# scanner decides whether the position/column test holds):
start_of_line          ::= ""   # <start-of-line>
end_of_input           ::= ""   # <end-of-input>
s_indent_less_than     ::= ""   # true iff current indent depth < recorded column k
s_indent_less_or_equal ::= ""   # true iff current indent depth <= recorded column k
```

In the context-free grammar below, every occurrence of `s-indent(n)` in the
original spec maps to a token match on `INDENT` at the appropriate depth;
`s-indent-less-than(n)` and `s-indent-less-or-equal(n)` are replaced by
regular expressions over `s_space` bounded by the token stream.

### Structural rules (unfolded)

```ebnf
s_separate_in_line ::=
    s_white+
  | start_of_line

s_block_line_prefix ::= INDENT
s_flow_line_prefix  ::= INDENT s_separate_in_line?

# l_empty, one per context:
l_empty_BLOCK_OUT ::= (s_block_line_prefix | s_indent_less_than) b_as_line_feed
l_empty_BLOCK_IN  ::= (s_block_line_prefix | s_indent_less_than) b_as_line_feed
l_empty_FLOW_OUT  ::= (s_flow_line_prefix | s_indent_less_than) b_as_line_feed
l_empty_FLOW_IN   ::= (s_flow_line_prefix | s_indent_less_than) b_as_line_feed
l_empty_BLOCK_KEY ::= (s_flow_line_prefix | s_indent_less_than) b_as_line_feed
l_empty_FLOW_KEY  ::= (s_flow_line_prefix | s_indent_less_than) b_as_line_feed

b_l_trimmed_BLOCK_OUT ::= b_non_content l_empty_BLOCK_OUT+
b_l_trimmed_BLOCK_IN  ::= b_non_content l_empty_BLOCK_IN+
b_l_trimmed_FLOW_OUT  ::= b_non_content l_empty_FLOW_OUT+
b_l_trimmed_FLOW_IN   ::= b_non_content l_empty_FLOW_IN+

b_as_space ::= b_break

b_l_folded_BLOCK_OUT ::= b_l_trimmed_BLOCK_OUT | b_as_space
b_l_folded_BLOCK_IN  ::= b_l_trimmed_BLOCK_IN  | b_as_space
b_l_folded_FLOW_OUT  ::= b_l_trimmed_FLOW_OUT  | b_as_space
b_l_folded_FLOW_IN   ::= b_l_trimmed_FLOW_IN   | b_as_space

s_flow_folded ::=
  s_separate_in_line?
  b_l_folded_FLOW_IN
  s_flow_line_prefix

c_nb_comment_text ::=
  c_comment
  nb_char*

b_comment ::=
    b_non_content
  | end_of_input

s_b_comment ::=
  (s_separate_in_line c_nb_comment_text?)?
  b_comment

l_comment ::=
  s_separate_in_line
  c_nb_comment_text?
  b_comment

s_l_comments ::=
  (s_b_comment | start_of_line)
  l_comment*

# s-separate, one per context:
s_separate_BLOCK_OUT ::= s_separate_lines
s_separate_BLOCK_IN  ::= s_separate_lines
s_separate_FLOW_OUT  ::= s_separate_lines
s_separate_FLOW_IN   ::= s_separate_lines
s_separate_BLOCK_KEY ::= s_separate_in_line
s_separate_FLOW_KEY  ::= s_separate_in_line

s_separate_lines ::=
    (s_l_comments s_flow_line_prefix)
  | s_separate_in_line
```

### Directives

```ebnf
l_directive ::=
  c_directive
  (ns_yaml_directive | ns_tag_directive | ns_reserved_directive)
  s_l_comments

ns_reserved_directive ::=
  ns_directive_name
  (s_separate_in_line ns_directive_parameter)*

ns_directive_name     ::= ns_char+
ns_directive_parameter ::= ns_char+

ns_yaml_directive ::=
  'YAML'
  s_separate_in_line
  ns_yaml_version

ns_yaml_version ::=
  ns_dec_digit+ '.' ns_dec_digit+

ns_tag_directive ::=
  'TAG'
  s_separate_in_line
  c_tag_handle
  s_separate_in_line
  ns_tag_prefix

c_tag_handle ::=
    c_named_tag_handle
  | c_secondary_tag_handle
  | c_primary_tag_handle

c_primary_tag_handle   ::= '!'
c_secondary_tag_handle ::= '!!'
c_named_tag_handle     ::= c_tag ns_word_char+ c_tag

ns_tag_prefix ::=
    c_ns_local_tag_prefix
  | ns_global_tag_prefix

c_ns_local_tag_prefix ::=
  c_tag
  ns_uri_char*

ns_global_tag_prefix ::=
  ns_tag_char
  ns_uri_char*

# c-ns-properties, one per context:
c_ns_properties_BLOCK_OUT ::=
    (c_ns_tag_property (s_separate_BLOCK_OUT c_ns_anchor_property)?)
  | (c_ns_anchor_property (s_separate_BLOCK_OUT c_ns_tag_property)?)

c_ns_properties_BLOCK_IN ::=
    (c_ns_tag_property (s_separate_BLOCK_IN c_ns_anchor_property)?)
  | (c_ns_anchor_property (s_separate_BLOCK_IN c_ns_tag_property)?)

c_ns_properties_FLOW_OUT ::=
    (c_ns_tag_property (s_separate_FLOW_OUT c_ns_anchor_property)?)
  | (c_ns_anchor_property (s_separate_FLOW_OUT c_ns_tag_property)?)

c_ns_properties_FLOW_IN ::=
    (c_ns_tag_property (s_separate_FLOW_IN c_ns_anchor_property)?)
  | (c_ns_anchor_property (s_separate_FLOW_IN c_ns_tag_property)?)

c_ns_properties_BLOCK_KEY ::=
    (c_ns_tag_property (s_separate_BLOCK_KEY c_ns_anchor_property)?)
  | (c_ns_anchor_property (s_separate_BLOCK_KEY c_ns_tag_property)?)

c_ns_properties_FLOW_KEY ::=
    (c_ns_tag_property (s_separate_FLOW_KEY c_ns_anchor_property)?)
  | (c_ns_anchor_property (s_separate_FLOW_KEY c_ns_tag_property)?)

c_ns_tag_property ::=
    c_verbatim_tag
  | c_ns_shorthand_tag
  | c_non_specific_tag

c_verbatim_tag ::=
  '!<' ns_uri_char+ '>'

c_ns_shorthand_tag ::=
  c_tag_handle ns_tag_char+

c_non_specific_tag ::= '!'

c_ns_anchor_property ::=
  c_anchor
  ns_anchor_name

ns_anchor_char ::= ns_char - c_flow_indicator
ns_anchor_name ::= ns_anchor_char+
```

## 7. Flow Style Productions — Context-Free Encoding

All productions parameterized by `c` ∈ {`BLOCK-OUT`, `BLOCK-IN`, `FLOW-OUT`, `FLOW-IN`, `BLOCK-KEY`, `FLOW-KEY`} and `n` (indentation, supplied by the lexer's `INDENT`/`DEDENT` token stream) are expanded into named nonterminals. `n+1` is written as `n+1` and resolved at the scanner level; only the finite context set is unfolded in the grammar.

```ebnf
c_ns_alias_node ::=
  c_alias
  ns_anchor_name

e_scalar ::= ""
e_node   ::= e_scalar

nb_double_char ::=
    c_ns_esc_char
  | (nb_json - c_escape - c_double_quote)

ns_double_char ::= nb_double_char - s_white

# c-double-quoted, one per content context:
c_double_quoted_FLOW_OUT  ::= c_double_quote nb_double_text_FLOW_OUT  c_double_quote
c_double_quoted_FLOW_IN   ::= c_double_quote nb_double_text_FLOW_IN   c_double_quote
c_double_quoted_BLOCK_KEY ::= c_double_quote nb_double_text_BLOCK_KEY c_double_quote
c_double_quoted_FLOW_KEY  ::= c_double_quote nb_double_text_FLOW_KEY  c_double_quote

# nb-double-text, one per context (the spec table, unfolded):
nb_double_text_FLOW_OUT  ::= nb_double_multi_line
nb_double_text_FLOW_IN   ::= nb_double_multi_line
nb_double_text_BLOCK_KEY ::= nb_double_one_line
nb_double_text_FLOW_KEY  ::= nb_double_one_line

nb_double_one_line ::= nb_double_char*

s_double_escaped ::=
  s_white*
  c_escape
  b_non_content
  l_empty_FLOW_IN*
  s_flow_line_prefix

s_double_break ::=
    s_double_escaped
  | s_flow_folded

nb_ns_double_in_line ::=
  ( s_white* ns_double_char )*

s_double_next_line ::=
  s_double_break
  (
      ns_double_char
      nb_ns_double_in_line
      ( s_double_next_line | s_white* )
  )?

nb_double_multi_line ::=
  nb_ns_double_in_line
  ( s_double_next_line | s_white* )

c_quoted_quote ::= "''"

nb_single_char ::=
    c_quoted_quote
  | ( nb_json - c_single_quote )

ns_single_char ::= nb_single_char - s_white

# c-single-quoted, one per content context:
c_single_quoted_FLOW_OUT  ::= c_single_quote nb_single_text_FLOW_OUT  c_single_quote
c_single_quoted_FLOW_IN   ::= c_single_quote nb_single_text_FLOW_IN   c_single_quote
c_single_quoted_BLOCK_KEY ::= c_single_quote nb_single_text_BLOCK_KEY c_single_quote
c_single_quoted_FLOW_KEY  ::= c_single_quote nb_single_text_FLOW_KEY  c_single_quote

# nb-single-text, one per context:
nb_single_text_FLOW_OUT  ::= nb_single_multi_line
nb_single_text_FLOW_IN   ::= nb_single_multi_line
nb_single_text_BLOCK_KEY ::= nb_single_one_line
nb_single_text_FLOW_KEY  ::= nb_single_one_line

nb_single_one_line ::= nb_single_char*

nb_ns_single_in_line ::=
  ( s_white* ns_single_char )*

s_single_next_line ::=
  s_flow_folded
  (
      ns_single_char
      nb_ns_single_in_line
      ( s_single_next_line | s_white* )
  )?

nb_single_multi_line ::=
  nb_ns_single_in_line
  ( s_single_next_line | s_white* )

# Plain style. The [ lookahead ] / [ lookbehind ] tests are folded into the
# character classes per the encoding rules (see header):
#  * the "indicator followed by plain-safe" lookahead is inlined by
#    concatenating ns_plain_safe directly after the indicator in ns_plain_first;
#  * the "':' followed by plain-safe" lookahead is inlined the same way;
#  * the "'#' with lookbehind ns-char" test is folded into nb_ns_plain_in_line,
#    where a '#' is only ever reached after at least one ns_plain_char.

# ns-plain-first, one per context that uses it:
ns_plain_first_BLOCK_OUT ::=
    ( ns_char - c_indicator )
  | ( ( c_mapping_key | c_mapping_value | c_sequence_entry ) ns_plain_safe_out )

ns_plain_first_BLOCK_IN ::=
    ( ns_char - c_indicator )
  | ( ( c_mapping_key | c_mapping_value | c_sequence_entry ) ns_plain_safe_in )

ns_plain_first_BLOCK_KEY ::=
    ( ns_char - c_indicator )
  | ( ( c_mapping_key | c_mapping_value | c_sequence_entry ) ns_plain_safe_out )

ns_plain_first_FLOW_OUT ::=
    ( ns_char - c_indicator )
  | ( ( c_mapping_key | c_mapping_value | c_sequence_entry ) ns_plain_safe_out )

ns_plain_first_FLOW_IN ::=
    ( ns_char - c_indicator )
  | ( ( c_mapping_key | c_mapping_value | c_sequence_entry ) ns_plain_safe_in )

ns_plain_first_FLOW_KEY ::=
    ( ns_char - c_indicator )
  | ( ( c_mapping_key | c_mapping_value | c_sequence_entry ) ns_plain_safe_in )

# ns-plain-safe, one per context (the spec table, unfolded):
ns_plain_safe_out ::= ns_char
ns_plain_safe_in  ::= ns_char - c_flow_indicator

ns_plain_safe_BLOCK_OUT ::= ns_plain_safe_out
ns_plain_safe_BLOCK_IN  ::= ns_plain_safe_in
ns_plain_safe_BLOCK_KEY ::= ns_plain_safe_out
ns_plain_safe_FLOW_KEY  ::= ns_plain_safe_in

# ns-plain-char, per context (lookahead folded as above):
ns_plain_char_BLOCK_OUT ::=
    ( ns_plain_safe_out - c_mapping_value - c_comment )
  | ( c_mapping_value ns_plain_safe_out )

ns_plain_char_BLOCK_IN ::=
    ( ns_plain_safe_in - c_mapping_value - c_comment )
  | ( c_mapping_value ns_plain_safe_in )

ns_plain_char_BLOCK_KEY ::=
    ( ns_plain_safe_out - c_mapping_value - c_comment )
  | ( c_mapping_value ns_plain_safe_out )

ns_plain_char_FLOW_OUT ::=
    ( ns_plain_safe_out - c_mapping_value - c_comment )
  | ( c_mapping_value ns_plain_safe_out )

ns_plain_char_FLOW_IN ::=
    ( ns_plain_safe_in - c_mapping_value - c_comment )
  | ( c_mapping_value ns_plain_safe_in )

ns_plain_char_FLOW_KEY ::=
    ( ns_plain_safe_in - c_mapping_value - c_comment )
  | ( c_mapping_value ns_plain_safe_in )

# ns-plain, one per context:
ns_plain_FLOW_OUT  ::= ns_plain_multi_line_FLOW_OUT
ns_plain_FLOW_IN   ::= ns_plain_multi_line_FLOW_IN
ns_plain_BLOCK_KEY ::= ns_plain_one_line_BLOCK_KEY
ns_plain_FLOW_KEY  ::= ns_plain_one_line_FLOW_KEY

# The spec's ns-plain-char third alternative ("# with lookbehind ns-char") is
# encoded in the in-line tail class: the c_comment branch is never preceded by
# s_white, so a hash is only absorbed into a plain scalar when it directly
# follows a plain char (a#b is one scalar), while " #" still closes the scalar
# and starts a comment.
nb_ns_plain_in_line_BLOCK_OUT ::=
  ( s_white* ns_plain_char_BLOCK_OUT | c_comment )*
nb_ns_plain_in_line_BLOCK_IN ::=
  ( s_white* ns_plain_char_BLOCK_IN | c_comment )*
nb_ns_plain_in_line_BLOCK_KEY ::=
  ( s_white* ns_plain_char_BLOCK_KEY | c_comment )*
nb_ns_plain_in_line_FLOW_OUT ::=
  ( s_white* ns_plain_char_FLOW_OUT | c_comment )*
nb_ns_plain_in_line_FLOW_IN ::=
  ( s_white* ns_plain_char_FLOW_IN | c_comment )*
nb_ns_plain_in_line_FLOW_KEY ::=
  ( s_white* ns_plain_char_FLOW_KEY | c_comment )*

# ns_plain_one_line (all six contexts) is defined once, further below.
s_ns_plain_next_line_BLOCK_OUT ::=
  s_flow_folded
  ns_plain_char_BLOCK_OUT
  nb_ns_plain_in_line_BLOCK_OUT
s_ns_plain_next_line_BLOCK_IN ::=
  s_flow_folded
  ns_plain_char_BLOCK_IN
  nb_ns_plain_in_line_BLOCK_IN
s_ns_plain_next_line_BLOCK_KEY ::=
  s_flow_folded
  ns_plain_char_BLOCK_KEY
  nb_ns_plain_in_line_BLOCK_KEY
s_ns_plain_next_line_FLOW_OUT ::=
  s_flow_folded
  ns_plain_char_FLOW_OUT
  nb_ns_plain_in_line_FLOW_OUT
s_ns_plain_next_line_FLOW_IN ::=
  s_flow_folded
  ns_plain_char_FLOW_IN
  nb_ns_plain_in_line_FLOW_IN
s_ns_plain_next_line_FLOW_KEY ::=
  s_flow_folded
  ns_plain_char_FLOW_KEY
  nb_ns_plain_in_line_FLOW_KEY

ns_plain_one_line_BLOCK_OUT ::=
  ns_plain_first_BLOCK_OUT
  nb_ns_plain_in_line_BLOCK_OUT
ns_plain_one_line_BLOCK_IN ::=
  ns_plain_first_BLOCK_IN
  nb_ns_plain_in_line_BLOCK_IN
ns_plain_one_line_BLOCK_KEY ::=
  ns_plain_first_BLOCK_KEY
  nb_ns_plain_in_line_BLOCK_KEY
ns_plain_one_line_FLOW_OUT ::=
  ns_plain_first_FLOW_OUT
  nb_ns_plain_in_line_FLOW_OUT
ns_plain_one_line_FLOW_IN ::=
  ns_plain_first_FLOW_IN
  nb_ns_plain_in_line_FLOW_IN
ns_plain_one_line_FLOW_KEY ::=
  ns_plain_first_FLOW_KEY
  nb_ns_plain_in_line_FLOW_KEY

ns_plain_multi_line_BLOCK_OUT ::=
  ns_plain_one_line_BLOCK_OUT
  s_ns_plain_next_line_BLOCK_OUT*
ns_plain_multi_line_BLOCK_IN ::=
  ns_plain_one_line_BLOCK_IN
  s_ns_plain_next_line_BLOCK_IN*
ns_plain_multi_line_BLOCK_KEY ::=
  ns_plain_one_line_BLOCK_KEY
  s_ns_plain_next_line_BLOCK_KEY*
ns_plain_multi_line_FLOW_OUT ::=
  ns_plain_one_line_FLOW_OUT
  s_ns_plain_next_line_FLOW_OUT*
ns_plain_multi_line_FLOW_IN ::=
  ns_plain_one_line_FLOW_IN
  s_ns_plain_next_line_FLOW_IN*
ns_plain_multi_line_FLOW_KEY ::=
  ns_plain_one_line_FLOW_KEY
  s_ns_plain_next_line_FLOW_KEY*
```
### Flow collections (unfolded per context)

```ebnf
# in-flow, one per context (the spec table, unfolded):
in_flow_FLOW_OUT  ::= ns_s_flow_seq_entries_FLOW_IN
in_flow_FLOW_IN   ::= ns_s_flow_seq_entries_FLOW_IN
in_flow_BLOCK_KEY ::= ns_s_flow_seq_entries_FLOW_KEY
in_flow_FLOW_KEY  ::= ns_s_flow_seq_entries_FLOW_KEY

c_flow_sequence_FLOW_OUT ::=
  c_sequence_start s_separate_FLOW_OUT? in_flow_FLOW_OUT? c_sequence_end
c_flow_sequence_FLOW_IN ::=
  c_sequence_start s_separate_FLOW_IN? in_flow_FLOW_IN? c_sequence_end
c_flow_sequence_BLOCK_KEY ::=
  c_sequence_start s_separate_BLOCK_KEY? in_flow_BLOCK_KEY? c_sequence_end
c_flow_sequence_FLOW_KEY ::=
  c_sequence_start s_separate_FLOW_KEY? in_flow_FLOW_KEY? c_sequence_end

ns_s_flow_seq_entries_FLOW_OUT ::=
  ns_flow_seq_entry_FLOW_OUT
  s_separate_FLOW_OUT?
  ( c_collect_entry s_separate_FLOW_OUT? ns_s_flow_seq_entries_FLOW_OUT? )?
ns_s_flow_seq_entries_FLOW_IN ::=
  ns_flow_seq_entry_FLOW_IN
  s_separate_FLOW_IN?
  ( c_collect_entry s_separate_FLOW_IN? ns_s_flow_seq_entries_FLOW_IN? )?
ns_s_flow_seq_entries_BLOCK_KEY ::=
  ns_flow_seq_entry_BLOCK_KEY
  s_separate_BLOCK_KEY?
  ( c_collect_entry s_separate_BLOCK_KEY? ns_s_flow_seq_entries_BLOCK_KEY? )?
ns_s_flow_seq_entries_FLOW_KEY ::=
  ns_flow_seq_entry_FLOW_KEY
  s_separate_FLOW_KEY?
  ( c_collect_entry s_separate_FLOW_KEY? ns_s_flow_seq_entries_FLOW_KEY? )?

ns_flow_seq_entry_FLOW_OUT ::= ns_flow_pair_FLOW_OUT  | ns_flow_node_FLOW_OUT
ns_flow_seq_entry_FLOW_IN  ::= ns_flow_pair_FLOW_IN  | ns_flow_node_FLOW_IN
ns_flow_seq_entry_BLOCK_KEY ::= ns_flow_pair_BLOCK_KEY | ns_flow_node_BLOCK_KEY
ns_flow_seq_entry_FLOW_KEY ::= ns_flow_pair_FLOW_KEY  | ns_flow_node_FLOW_KEY

# c-flow-mapping: ns-s-flow-map-entries uses in-flow(c) per the spec table:
c_flow_mapping_FLOW_OUT ::=
  c_mapping_start s_separate_FLOW_OUT? ns_s_flow_map_entries_FLOW_IN? c_mapping_end
c_flow_mapping_FLOW_IN ::=
  c_mapping_start s_separate_FLOW_IN? ns_s_flow_map_entries_FLOW_IN? c_mapping_end
c_flow_mapping_BLOCK_KEY ::=
  c_mapping_start s_separate_BLOCK_KEY? ns_s_flow_map_entries_FLOW_KEY? c_mapping_end
c_flow_mapping_FLOW_KEY ::=
  c_mapping_start s_separate_FLOW_KEY? ns_s_flow_map_entries_FLOW_KEY? c_mapping_end

ns_s_flow_map_entries_FLOW_OUT ::=
  ns_flow_map_entry_FLOW_OUT
  s_separate_FLOW_OUT?
  ( c_collect_entry s_separate_FLOW_OUT? ns_s_flow_map_entries_FLOW_OUT? )?
ns_s_flow_map_entries_FLOW_IN ::=
  ns_flow_map_entry_FLOW_IN
  s_separate_FLOW_IN?
  ( c_collect_entry s_separate_FLOW_IN? ns_s_flow_map_entries_FLOW_IN? )?
ns_s_flow_map_entries_BLOCK_KEY ::=
  ns_flow_map_entry_BLOCK_KEY
  s_separate_BLOCK_KEY?
  ( c_collect_entry s_separate_BLOCK_KEY? ns_s_flow_map_entries_BLOCK_KEY? )?
ns_s_flow_map_entries_FLOW_KEY ::=
  ns_flow_map_entry_FLOW_KEY
  s_separate_FLOW_KEY?
  ( c_collect_entry s_separate_FLOW_KEY? ns_s_flow_map_entries_FLOW_KEY? )?

ns_flow_map_entry_FLOW_OUT ::=
    ( c_mapping_key s_separate_FLOW_OUT ns_flow_map_explicit_entry_FLOW_OUT )
  | ns_flow_map_implicit_entry_FLOW_OUT
ns_flow_map_entry_FLOW_IN ::=
    ( c_mapping_key s_separate_FLOW_IN ns_flow_map_explicit_entry_FLOW_IN )
  | ns_flow_map_implicit_entry_FLOW_IN
ns_flow_map_entry_BLOCK_KEY ::=
    ( c_mapping_key s_separate_BLOCK_KEY ns_flow_map_explicit_entry_BLOCK_KEY )
  | ns_flow_map_implicit_entry_BLOCK_KEY
ns_flow_map_entry_FLOW_KEY ::=
    ( c_mapping_key s_separate_FLOW_KEY ns_flow_map_explicit_entry_FLOW_KEY )
  | ns_flow_map_implicit_entry_FLOW_KEY

ns_flow_map_explicit_entry_FLOW_OUT  ::= ns_flow_map_implicit_entry_FLOW_OUT  | ( e_node e_node )
ns_flow_map_explicit_entry_FLOW_IN   ::= ns_flow_map_implicit_entry_FLOW_IN   | ( e_node e_node )
ns_flow_map_explicit_entry_BLOCK_KEY ::= ns_flow_map_implicit_entry_BLOCK_KEY | ( e_node e_node )
ns_flow_map_explicit_entry_FLOW_KEY  ::= ns_flow_map_implicit_entry_FLOW_KEY  | ( e_node e_node )

ns_flow_map_implicit_entry_FLOW_OUT ::=
    ns_flow_map_yaml_key_entry_FLOW_OUT
  | c_ns_flow_map_empty_key_entry_FLOW_OUT
  | c_ns_flow_map_json_key_entry_FLOW_OUT
ns_flow_map_implicit_entry_FLOW_IN ::=
    ns_flow_map_yaml_key_entry_FLOW_IN
  | c_ns_flow_map_empty_key_entry_FLOW_IN
  | c_ns_flow_map_json_key_entry_FLOW_IN
ns_flow_map_implicit_entry_BLOCK_KEY ::=
    ns_flow_map_yaml_key_entry_BLOCK_KEY
  | c_ns_flow_map_empty_key_entry_BLOCK_KEY
  | c_ns_flow_map_json_key_entry_BLOCK_KEY
ns_flow_map_implicit_entry_FLOW_KEY ::=
    ns_flow_map_yaml_key_entry_FLOW_KEY
  | c_ns_flow_map_empty_key_entry_FLOW_KEY
  | c_ns_flow_map_json_key_entry_FLOW_KEY

ns_flow_map_yaml_key_entry_FLOW_OUT ::=
  ns_flow_yaml_node_FLOW_OUT
  ( ( s_separate_FLOW_OUT? c_ns_flow_map_separate_value_FLOW_OUT ) | e_node )
ns_flow_map_yaml_key_entry_FLOW_IN ::=
  ns_flow_yaml_node_FLOW_IN
  ( ( s_separate_FLOW_IN? c_ns_flow_map_separate_value_FLOW_IN ) | e_node )
ns_flow_map_yaml_key_entry_BLOCK_KEY ::=
  ns_flow_yaml_node_BLOCK_KEY
  ( ( s_separate_BLOCK_KEY? c_ns_flow_map_separate_value_BLOCK_KEY ) | e_node )
ns_flow_map_yaml_key_entry_FLOW_KEY ::=
  ns_flow_yaml_node_FLOW_KEY
  ( ( s_separate_FLOW_KEY? c_ns_flow_map_separate_value_FLOW_KEY ) | e_node )

c_ns_flow_map_empty_key_entry_FLOW_OUT  ::= e_node c_ns_flow_map_separate_value_FLOW_OUT
c_ns_flow_map_empty_key_entry_FLOW_IN   ::= e_node c_ns_flow_map_separate_value_FLOW_IN
c_ns_flow_map_empty_key_entry_BLOCK_KEY ::= e_node c_ns_flow_map_separate_value_BLOCK_KEY
c_ns_flow_map_empty_key_entry_FLOW_KEY  ::= e_node c_ns_flow_map_separate_value_FLOW_KEY

# The ":" [ lookahead != ns-plain-safe(c) ] test is folded into a guard class
# (the document's stated convention): the value after ": " may be empty, but a
# plain-scalar value may not begin with the flow-unsafe char that would be
# ambiguous. We encode the lookahead as the char class "not the plain-safe
# start of c" — i.e. ':' must NOT be immediately followed by ns-plain-safe(c),
# which is enforced by requiring either a separator then a node, or e-node.
c_ns_flow_map_separate_value_FLOW_OUT ::=
  c_mapping_value
  ( ( s_separate_FLOW_OUT ns_flow_node_FLOW_OUT ) | e_node )
c_ns_flow_map_separate_value_FLOW_IN ::=
  c_mapping_value
  ( ( s_separate_FLOW_IN ns_flow_node_FLOW_IN ) | e_node )
c_ns_flow_map_separate_value_BLOCK_KEY ::=
  c_mapping_value
  ( ( s_separate_BLOCK_KEY ns_flow_node_BLOCK_KEY ) | e_node )
c_ns_flow_map_separate_value_FLOW_KEY ::=
  c_mapping_value
  ( ( s_separate_FLOW_KEY ns_flow_node_FLOW_KEY ) | e_node )

c_ns_flow_map_json_key_entry_FLOW_OUT ::=
  c_flow_json_node_FLOW_OUT
  ( ( s_separate_FLOW_OUT? c_ns_flow_map_adjacent_value_FLOW_OUT ) | e_node )
c_ns_flow_map_json_key_entry_FLOW_IN ::=
  c_flow_json_node_FLOW_IN
  ( ( s_separate_FLOW_IN? c_ns_flow_map_adjacent_value_FLOW_IN ) | e_node )
c_ns_flow_map_json_key_entry_BLOCK_KEY ::=
  c_flow_json_node_BLOCK_KEY
  ( ( s_separate_BLOCK_KEY? c_ns_flow_map_adjacent_value_BLOCK_KEY ) | e_node )
c_ns_flow_map_json_key_entry_FLOW_KEY ::=
  c_flow_json_node_FLOW_KEY
  ( ( s_separate_FLOW_KEY? c_ns_flow_map_adjacent_value_FLOW_KEY ) | e_node )

c_ns_flow_map_adjacent_value_FLOW_OUT ::=
  c_mapping_value ( ( s_separate_FLOW_OUT? ns_flow_node_FLOW_OUT ) | e_node )
c_ns_flow_map_adjacent_value_FLOW_IN ::=
  c_mapping_value ( ( s_separate_FLOW_IN? ns_flow_node_FLOW_IN ) | e_node )
c_ns_flow_map_adjacent_value_BLOCK_KEY ::=
  c_mapping_value ( ( s_separate_BLOCK_KEY? ns_flow_node_BLOCK_KEY ) | e_node )
c_ns_flow_map_adjacent_value_FLOW_KEY ::=
  c_mapping_value ( ( s_separate_FLOW_KEY? ns_flow_node_FLOW_KEY ) | e_node )

ns_flow_pair_FLOW_OUT ::=
    ( c_mapping_key s_separate_FLOW_OUT ns_flow_map_explicit_entry_FLOW_OUT )
  | ns_flow_pair_entry_FLOW_OUT
ns_flow_pair_FLOW_IN ::=
    ( c_mapping_key s_separate_FLOW_IN ns_flow_map_explicit_entry_FLOW_IN )
  | ns_flow_pair_entry_FLOW_IN
ns_flow_pair_BLOCK_KEY ::=
    ( c_mapping_key s_separate_BLOCK_KEY ns_flow_map_explicit_entry_BLOCK_KEY )
  | ns_flow_pair_entry_BLOCK_KEY
ns_flow_pair_FLOW_KEY ::=
    ( c_mapping_key s_separate_FLOW_KEY ns_flow_map_explicit_entry_FLOW_KEY )
  | ns_flow_pair_entry_FLOW_KEY

ns_flow_pair_entry_FLOW_OUT ::=
    ns_flow_pair_yaml_key_entry_FLOW_OUT
  | c_ns_flow_map_empty_key_entry_FLOW_OUT
  | c_ns_flow_pair_json_key_entry_FLOW_OUT
ns_flow_pair_entry_FLOW_IN ::=
    ns_flow_pair_yaml_key_entry_FLOW_IN
  | c_ns_flow_map_empty_key_entry_FLOW_IN
  | c_ns_flow_pair_json_key_entry_FLOW_IN
ns_flow_pair_entry_BLOCK_KEY ::=
    ns_flow_pair_yaml_key_entry_BLOCK_KEY
  | c_ns_flow_map_empty_key_entry_BLOCK_KEY
  | c_ns_flow_pair_json_key_entry_BLOCK_KEY
ns_flow_pair_entry_FLOW_KEY ::=
    ns_flow_pair_yaml_key_entry_FLOW_KEY
  | c_ns_flow_map_empty_key_entry_FLOW_KEY
  | c_ns_flow_pair_json_key_entry_FLOW_KEY

ns_flow_pair_yaml_key_entry_FLOW_OUT ::=
  ns_s_implicit_yaml_key_FLOW_KEY c_ns_flow_map_separate_value_FLOW_OUT
ns_flow_pair_yaml_key_entry_FLOW_IN ::=
  ns_s_implicit_yaml_key_FLOW_KEY c_ns_flow_map_separate_value_FLOW_IN
ns_flow_pair_yaml_key_entry_BLOCK_KEY ::=
  ns_s_implicit_yaml_key_FLOW_KEY c_ns_flow_map_separate_value_BLOCK_KEY
ns_flow_pair_yaml_key_entry_FLOW_KEY ::=
  ns_s_implicit_yaml_key_FLOW_KEY c_ns_flow_map_separate_value_FLOW_KEY

c_ns_flow_pair_json_key_entry_FLOW_OUT ::=
  c_s_implicit_json_key_FLOW_KEY c_ns_flow_map_adjacent_value_FLOW_OUT
c_ns_flow_pair_json_key_entry_FLOW_IN ::=
  c_s_implicit_json_key_FLOW_KEY c_ns_flow_map_adjacent_value_FLOW_IN
c_ns_flow_pair_json_key_entry_BLOCK_KEY ::=
  c_s_implicit_json_key_FLOW_KEY c_ns_flow_map_adjacent_value_BLOCK_KEY
c_ns_flow_pair_json_key_entry_FLOW_KEY ::=
  c_s_implicit_json_key_FLOW_KEY c_ns_flow_map_adjacent_value_FLOW_KEY

ns_s_implicit_yaml_key_FLOW_OUT ::= ns_flow_yaml_node_0_FLOW_OUT  s_separate_in_line?
ns_s_implicit_yaml_key_FLOW_IN  ::= ns_flow_yaml_node_0_FLOW_IN   s_separate_in_line?
ns_s_implicit_yaml_key_BLOCK_KEY ::= ns_flow_yaml_node_0_BLOCK_KEY s_separate_in_line?
ns_s_implicit_yaml_key_FLOW_KEY ::= ns_flow_yaml_node_0_FLOW_KEY  s_separate_in_line?

c_s_implicit_json_key_FLOW_OUT  ::= c_flow_json_node_0_FLOW_OUT  s_separate_in_line?
c_s_implicit_json_key_FLOW_IN   ::= c_flow_json_node_0_FLOW_IN   s_separate_in_line?
c_s_implicit_json_key_BLOCK_KEY ::= c_flow_json_node_0_BLOCK_KEY s_separate_in_line?
c_s_implicit_json_key_FLOW_KEY  ::= c_flow_json_node_0_FLOW_KEY  s_separate_in_line?

ns_flow_yaml_content_FLOW_OUT ::= ns_plain_FLOW_OUT
ns_flow_yaml_content_FLOW_IN  ::= ns_plain_FLOW_IN
ns_flow_yaml_content_BLOCK_KEY ::= ns_plain_BLOCK_KEY
ns_flow_yaml_content_FLOW_KEY ::= ns_plain_FLOW_KEY
# (with n = 0, ns_plain_* resolves to the one-line form via ns-plain(n))

c_flow_json_content_FLOW_OUT  ::= c_flow_sequence_FLOW_OUT | c_flow_mapping_FLOW_OUT | c_single_quoted_FLOW_OUT | c_double_quoted_FLOW_OUT
c_flow_json_content_FLOW_IN   ::= c_flow_sequence_FLOW_IN  | c_flow_mapping_FLOW_IN  | c_single_quoted_FLOW_IN  | c_double_quoted_FLOW_IN
c_flow_json_content_BLOCK_KEY ::= c_flow_sequence_BLOCK_KEY | c_flow_mapping_BLOCK_KEY | c_single_quoted_BLOCK_KEY | c_double_quoted_BLOCK_KEY
c_flow_json_content_FLOW_KEY  ::= c_flow_sequence_FLOW_KEY  | c_flow_mapping_FLOW_KEY  | c_single_quoted_FLOW_KEY  | c_double_quoted_FLOW_KEY

ns_flow_content_FLOW_OUT  ::= ns_flow_yaml_content_FLOW_OUT  | c_flow_json_content_FLOW_OUT
ns_flow_content_FLOW_IN   ::= ns_flow_yaml_content_FLOW_IN   | c_flow_json_content_FLOW_IN
ns_flow_content_BLOCK_KEY ::= ns_flow_yaml_content_BLOCK_KEY | c_flow_json_content_BLOCK_KEY
ns_flow_content_FLOW_KEY  ::= ns_flow_yaml_content_FLOW_KEY  | c_flow_json_content_FLOW_KEY

ns_flow_yaml_node_FLOW_OUT ::=
    c_ns_alias_node
  | ns_flow_yaml_content_FLOW_OUT
  | ( c_ns_properties_FLOW_OUT ( ( s_separate_FLOW_OUT ns_flow_yaml_content_FLOW_OUT ) | e_scalar ) )
ns_flow_yaml_node_FLOW_IN ::=
    c_ns_alias_node
  | ns_flow_yaml_content_FLOW_IN
  | ( c_ns_properties_FLOW_IN ( ( s_separate_FLOW_IN ns_flow_yaml_content_FLOW_IN ) | e_scalar ) )
ns_flow_yaml_node_BLOCK_KEY ::=
    c_ns_alias_node
  | ns_flow_yaml_content_BLOCK_KEY
  | ( c_ns_properties_BLOCK_KEY ( ( s_separate_BLOCK_KEY ns_flow_yaml_content_BLOCK_KEY ) | e_scalar ) )
ns_flow_yaml_node_FLOW_KEY ::=
    c_ns_alias_node
  | ns_flow_yaml_content_FLOW_KEY
  | ( c_ns_properties_FLOW_KEY ( ( s_separate_FLOW_KEY ns_flow_yaml_content_FLOW_KEY ) | e_scalar ) )

# n = 0 variants for the implicit-key rules:
ns_flow_yaml_node_0_FLOW_OUT  ::= ns_flow_yaml_node_FLOW_OUT
ns_flow_yaml_node_0_FLOW_IN   ::= ns_flow_yaml_node_FLOW_IN
ns_flow_yaml_node_0_BLOCK_KEY ::= ns_flow_yaml_node_BLOCK_KEY
ns_flow_yaml_node_0_FLOW_KEY  ::= ns_flow_yaml_node_FLOW_KEY

c_flow_json_node_FLOW_OUT ::=
  ( c_ns_properties_FLOW_OUT s_separate_FLOW_OUT )? c_flow_json_content_FLOW_OUT
c_flow_json_node_FLOW_IN ::=
  ( c_ns_properties_FLOW_IN s_separate_FLOW_IN )? c_flow_json_content_FLOW_IN
c_flow_json_node_BLOCK_KEY ::=
  ( c_ns_properties_BLOCK_KEY s_separate_BLOCK_KEY )? c_flow_json_content_BLOCK_KEY
c_flow_json_node_FLOW_KEY ::=
  ( c_ns_properties_FLOW_KEY s_separate_FLOW_KEY )? c_flow_json_content_FLOW_KEY

c_flow_json_node_0_FLOW_OUT  ::= c_flow_json_node_FLOW_OUT
c_flow_json_node_0_FLOW_IN   ::= c_flow_json_node_FLOW_IN
c_flow_json_node_0_BLOCK_KEY ::= c_flow_json_node_BLOCK_KEY
c_flow_json_node_0_FLOW_KEY  ::= c_flow_json_node_FLOW_KEY

ns_flow_node_FLOW_OUT ::=
    c_ns_alias_node
  | ns_flow_content_FLOW_OUT
  | ( c_ns_properties_FLOW_OUT ( ( s_separate_FLOW_OUT ns_flow_content_FLOW_OUT ) | e_scalar ) )
ns_flow_node_FLOW_IN ::=
    c_ns_alias_node
  | ns_flow_content_FLOW_IN
  | ( c_ns_properties_FLOW_IN ( ( s_separate_FLOW_IN ns_flow_content_FLOW_IN ) | e_scalar ) )
ns_flow_node_BLOCK_KEY ::=
    c_ns_alias_node
  | ns_flow_content_BLOCK_KEY
  | ( c_ns_properties_BLOCK_KEY ( ( s_separate_BLOCK_KEY ns_flow_content_BLOCK_KEY ) | e_scalar ) )
ns_flow_node_FLOW_KEY ::=
    c_ns_alias_node
  | ns_flow_content_FLOW_KEY
  | ( c_ns_properties_FLOW_KEY ( ( s_separate_FLOW_KEY ns_flow_content_FLOW_KEY ) | e_scalar ) )
```

## 8. Block Style Productions — Context-Free Encoding

Here the only parameters are the block context `c` ∈ {`BLOCK-IN`, `BLOCK-OUT`}
(the flow contexts never reach block style) and the chomping parameter
`t` ∈ {`STRIP`, `CLIP`, `KEEP`}. Both sets are finite, so each production is
written once per member as a suffixed nonterminal. The indentation parameters
(`n`, `n+1+m`, `n-1`) never appear in the grammar: `s-indent(k)` is a match on
the lexer's `INDENT` token at the column the scanner recorded, and the
`-less-than` / `-less-or-equal` forms are scanner predicates (see §6).

```ebnf
# s-indent(n) — token match supplied by the scanner (see §6 "Indentation")
s_indent ::= INDENT

# c-b-block-header(t) — one per chomping parameter:
c_b_block_header_STRIP ::=
    ( c_indentation_indicator c_chomping_indicator_STRIP
    | c_chomping_indicator_STRIP c_indentation_indicator )
    s_b_comment
c_b_block_header_KEEP  ::=
    ( c_indentation_indicator c_chomping_indicator_KEEP
    | c_chomping_indicator_KEEP c_indentation_indicator )
    s_b_comment
c_b_block_header_CLIP  ::=
    ( c_indentation_indicator c_chomping_indicator_CLIP
    | c_chomping_indicator_CLIP c_indentation_indicator )
    s_b_comment

# c-indentation-indicator — purely lexical; the scanner records its value as m,
# so the content indent is n+m and no parameter is carried in the grammar:
c_indentation_indicator ::= [x31 - x39]        # 1-9

# c-chomping-indicator(t) — spec table, unfolded:
c_chomping_indicator_STRIP ::= '-'
c_chomping_indicator_KEEP  ::= '+'
c_chomping_indicator_CLIP  ::= ""

# b-chomped-last(t) — spec table, unfolded:
b_chomped_last_STRIP ::= b_non_content  | end_of_input
b_chomped_last_CLIP  ::= b_as_line_feed | end_of_input
b_chomped_last_KEEP  ::= b_as_line_feed | end_of_input

# l-chomped-empty(n,t) — spec table, unfolded:
l_chomped_empty_STRIP ::= l_strip_empty
l_chomped_empty_CLIP  ::= l_strip_empty
l_chomped_empty_KEEP  ::= l_keep_empty

l_strip_empty ::=
    ( s_indent_less_or_equal b_non_content )*
    l_trail_comments?

l_keep_empty ::=
    l_empty_BLOCK_IN*
    l_trail_comments?

l_trail_comments ::=
    s_indent_less_than
    c_nb_comment_text
    b_comment
    l_comment*
```

### Literal and folded scalars (chomping unfolded)

```ebnf
# c-l+literal(n) — one per chomping parameter; content indent n+m from scanner:
c_l_literal_STRIP ::= c_literal c_b_block_header_STRIP l_literal_content_STRIP
c_l_literal_KEEP  ::= c_literal c_b_block_header_KEEP  l_literal_content_KEEP
c_l_literal_CLIP  ::= c_literal c_b_block_header_CLIP  l_literal_content_CLIP

l_literal_content_STRIP ::=
    ( l_nb_literal_text b_nb_literal_next* b_chomped_last_STRIP )?
    l_chomped_empty_STRIP
l_literal_content_KEEP  ::=
    ( l_nb_literal_text b_nb_literal_next* b_chomped_last_KEEP )?
    l_chomped_empty_KEEP
l_literal_content_CLIP  ::=
    ( l_nb_literal_text b_nb_literal_next* b_chomped_last_CLIP )?
    l_chomped_empty_CLIP

l_nb_literal_text ::= l_empty_BLOCK_IN* s_indent nb_char+
b_nb_literal_next ::= b_as_line_feed l_nb_literal_text

# c-l+folded(n) — one per chomping parameter:
c_l_folded_STRIP ::= c_folded c_b_block_header_STRIP l_folded_content_STRIP
c_l_folded_KEEP  ::= c_folded c_b_block_header_KEEP  l_folded_content_KEEP
c_l_folded_CLIP  ::= c_folded c_b_block_header_CLIP  l_folded_content_CLIP

s_nb_folded_text  ::= s_indent ns_char nb_char*
l_nb_folded_lines ::= s_nb_folded_text ( b_l_folded_BLOCK_IN s_nb_folded_text )*
s_nb_spaced_text  ::= s_indent s_white nb_char*
b_l_spaced        ::= b_as_line_feed l_empty_BLOCK_IN*
l_nb_spaced_lines ::= s_nb_spaced_text ( b_l_spaced s_nb_spaced_text )*
l_nb_same_lines   ::= l_empty_BLOCK_IN* ( l_nb_folded_lines | l_nb_spaced_lines )
l_nb_diff_lines   ::= l_nb_same_lines ( b_as_line_feed l_nb_same_lines )*

l_folded_content_STRIP ::= ( l_nb_diff_lines b_chomped_last_STRIP )? l_chomped_empty_STRIP
l_folded_content_KEEP  ::= ( l_nb_diff_lines b_chomped_last_KEEP )? l_chomped_empty_KEEP
l_folded_content_CLIP  ::= ( l_nb_diff_lines b_chomped_last_CLIP )? l_chomped_empty_CLIP

# The chomping families joined, as referenced by s-l+block-scalar below:
c_l_literal ::= c_l_literal_STRIP | c_l_literal_KEEP | c_l_literal_CLIP
c_l_folded  ::= c_l_folded_STRIP | c_l_folded_KEEP | c_l_folded_CLIP
```

### Block collections (unfolded per block context)

`c-l-block-seq-entry` carries the spec's `[ lookahead ≠ ns-char ]` test and the
`?` / `:` "not followed by non-ws char" tests. Those are folded into the lexer:
it only emits the `-`, `?`, `:` indicator token when it is not followed by a
non-space character, and every `s-l+block-*` right-hand side below starts with a
separator, so the CFG needs no back-reference.

```ebnf
l_block_sequence ::= ( s_indent c_l_block_seq_entry )+

c_l_block_seq_entry ::= c_sequence_entry s_l_block_indented_BLOCK_IN

s_l_block_indented_BLOCK_IN ::=
      ( s_indent ( ns_l_compact_sequence | ns_l_compact_mapping ) )
    | s_l_block_node_BLOCK_IN
    | ( e_node s_l_comments )
s_l_block_indented_BLOCK_OUT ::=
      ( s_indent ( ns_l_compact_sequence | ns_l_compact_mapping ) )
    | s_l_block_node_BLOCK_OUT
    | ( e_node s_l_comments )

ns_l_compact_sequence ::= c_l_block_seq_entry ( s_indent c_l_block_seq_entry )*

l_block_mapping    ::= ( s_indent ns_l_block_map_entry )+
ns_l_block_map_entry ::= c_l_block_map_explicit_entry | ns_l_block_map_implicit_entry

c_l_block_map_explicit_entry ::=
    c_l_block_map_explicit_key
    ( l_block_map_explicit_value | e_node )

c_l_block_map_explicit_key ::= c_mapping_key s_l_block_indented_BLOCK_OUT
l_block_map_explicit_value ::= s_indent c_mapping_value s_l_block_indented_BLOCK_OUT

ns_l_block_map_implicit_entry ::=
    ( ns_s_block_map_implicit_key | e_node )
    c_l_block_map_implicit_value

# BLOCK-KEY variants of the implicit-key macros, already unfolded in §7:
ns_s_block_map_implicit_key ::=
      c_s_implicit_json_key_BLOCK_KEY
    | ns_s_implicit_yaml_key_BLOCK_KEY

c_l_block_map_implicit_value ::=
    c_mapping_value
    ( s_l_block_node_BLOCK_OUT | ( e_node s_l_comments ) )

ns_l_compact_mapping ::= ns_l_block_map_entry ( s_indent ns_l_block_map_entry )*
```

### Node placement inside a block (context unfolded)

```ebnf
s_l_block_node_BLOCK_IN  ::= s_l_block_in_block_BLOCK_IN  | s_l_flow_in_block
s_l_block_node_BLOCK_OUT ::= s_l_block_in_block_BLOCK_OUT | s_l_flow_in_block

s_l_flow_in_block ::=
    s_separate_FLOW_OUT
    ns_flow_node_FLOW_OUT
    s_l_comments

s_l_block_in_block_BLOCK_IN  ::= s_l_block_scalar_BLOCK_IN  | s_l_block_collection_BLOCK_IN
s_l_block_in_block_BLOCK_OUT ::= s_l_block_scalar_BLOCK_OUT | s_l_block_collection_BLOCK_OUT

s_l_block_scalar_BLOCK_IN ::=
    s_separate_BLOCK_IN
    ( c_ns_properties_BLOCK_IN s_separate_BLOCK_IN )?
    ( c_l_literal | c_l_folded )

s_l_block_scalar_BLOCK_OUT ::=
    s_separate_BLOCK_OUT
    ( c_ns_properties_BLOCK_OUT s_separate_BLOCK_OUT )?
    ( c_l_literal | c_l_folded )

s_l_block_collection_BLOCK_IN ::=
    ( s_separate_BLOCK_IN c_ns_properties_BLOCK_IN )?
    s_l_comments
    ( seq_space_BLOCK_IN  | l_block_mapping )

s_l_block_collection_BLOCK_OUT ::=
    ( s_separate_BLOCK_OUT c_ns_properties_BLOCK_OUT )?
    s_l_comments
    ( seq_space_BLOCK_OUT | l_block_mapping )

# spec: seq-space(n,BLOCK-IN)  ::= l+block-sequence(n)
#       seq-space(n,BLOCK-OUT) ::= l+block-sequence(n-1)
# the "n-1" is the scanner matching INDENT at the outer column, so both contexts
# share the same context-free nonterminal:
seq_space_BLOCK_IN  ::= l_block_sequence
seq_space_BLOCK_OUT ::= l_block_sequence
```

## 9. Document Stream Productions — Context-Free Encoding

No parameters appear in this chapter; the only context-sensitivity is the
`c-forbidden` exclusion, which is a scanner constraint (a bare document may not
begin with a `---` or `...` marker line), so it is stated as a lexical rule and
`l-bare-document` needs no back-reference.

```ebnf
l_document_prefix ::=
    c_byte_order_mark?
    l_comment*

c_directives_end ::= '---'
c_document_end   ::= '...'      # not followed by a non-ws char: lexer token rule

l_document_suffix ::=
    c_document_end
    s_l_comments

# c-forbidden — scanner-level exclusion, kept for reference. The lexer refuses to
# begin a bare document when a line starts with a directives-end or document-end
# marker followed by a line break, white space or end-of-input:
c_forbidden ::=
    start_of_line
    ( c_directives_end | c_document_end )
    ( b_char | s_white | end_of_input )

l_bare_document ::=
    s_l_block_node_BLOCK_IN     # excluding c_forbidden content

l_explicit_document ::=
    c_directives_end
    ( l_bare_document | ( e_node s_l_comments ) )

l_directive_document ::=
    l_directive+
    l_explicit_document

l_any_document ::=
      l_directive_document
    | l_explicit_document
    | l_bare_document

# Start symbol: a byte stream is well-formed iff it derives l_yaml_stream.
l_yaml_stream ::=
    l_document_prefix*
    l_any_document?
    (
          ( l_document_suffix+ l_document_prefix* l_any_document? )
        | c_byte_order_mark
        | l_comment
        | l_explicit_document
    )*
```

## Coverage — cross-check of every spec production

Enumerating `yaml_grammar.md` gives **210 fenced production blocks** (the header
quotes 211 because the `s-indent` block carries two clauses, the `n = 0` base
case and the recursive step). Every one of the 210 is accounted for below:

| Spec chapter | Production blocks | CFG section | CFG nonterminals | Result |
| ------------ | ----------------: | ----------- | ---------------: | ------ |
| §5 Character Productions | 61 | `## §5 Character Productions — Context-Free Encoding` | 62 | 61/61 mapped (1 extra helper: `b_carriage_return`) |
| §6 Structural Productions | 41 | `## §6 Structural Productions — Context-Free Encoding` | 64 | 41/41 mapped (macro tables `s-line-prefix`, `s-separate` unfolded; `s-indent*` becomes the `INDENT`/`DEDENT` lexer tokens) |
| §7 Flow Style Productions | 58 | `## 7. Flow Style Productions — Context-Free Encoding` | 192 | 58/58 mapped (context tables unfolded) |
| §8 Block Style Productions | 40 | `## 8. Block Style Productions — Context-Free Encoding` | 65 | 40/40 mapped (5 macro tables unfolded) |
| §9 Document Stream Productions | 10 | `## 9. Document Stream Productions — Context-Free Encoding` | 10 | 10/10 mapped |
| **Total** | **210** | | **393** | **complete** |

Unfolding factor: 210 spec blocks produce 393 named nonterminals, i.e. the
`c`-parameterized families (up to 6 contexts), the `t`-parameterized families
(3 chomping modes) and the `n = 0` instantiations (`ns_flow_yaml_node_0_*`,
`c_flow_json_node_0_*`) of the implicit-key rules.

### Gaps found by the cross-check and fixed in this pass

1. **Undefined scanner predicates** — `s_indent_less_than` (used by `l_empty_*`
   and `l_trail_comments`) and `s_indent_less_or_equal` (used by `l_strip_empty`)
   had no definition anywhere. Added in §6 as empty-matching predicates whose
   column comparison is evaluated by the scanner.
2. **Undefined zero-width anchors** — `start_of_line` (`s_separate_in_line`,
   `s_l_comments`, `c_forbidden`) and `end_of_input` (`b_comment`,
   `b_chomped_last_*`) had no definition. Added in §6.
3. **Duplicate definitions** — `ns_plain_one_line_BLOCK_KEY` and
   `ns_plain_one_line_FLOW_KEY` were each defined twice in §7. The earlier pair
   was deleted; all six `ns_plain_one_line_*` contexts are now defined exactly
   once.
4. **Unmapped spec alternative** — the third alternative of `ns-plain-char(c)`,
   `[ lookbehind = ns-char ] c-comment`, was dropped, so plain scalars such as
   `a#b` were unparseable. It is now encoded inside `nb_ns_plain_in_line_*` as a
   `c_comment` branch that is never preceded by `s_white` (which also keeps
   `a #b` = scalar `a` + comment).
5. **Incomplete context family** — the `l_empty` family only covered the four
   content contexts; the two key-context rows of the spec's `l-empty(n,c)`
   production (`BLOCK-KEY`, `FLOW-KEY`) were added, so all six values of `c` now
   have a named nonterminal. They are currently unreferenced (no rule
   instantiates `l-empty` inside a key), and are kept for 1:1 name coverage.

### Deviations that are intentional, not defects

* `ns_plain_safe_out` / `ns_plain_safe_in` are the spec's `ns-plain-safe-out` /
  `ns-plain-safe-in`; the suffixed aliases cover `BLOCK_OUT`, `BLOCK_IN`,
  `BLOCK_KEY`, `FLOW_KEY`. `ns_plain_safe_BLOCK_OUT` / `ns_plain_safe_BLOCK_IN`
  are defined-but-unused conveniences, and the `FLOW_OUT` / `FLOW_IN` rows of the
  spec table use the base classes directly.
* `DEDENT` is declared for the lexer but only referenced in prose; the parser
  detects dedent by the `INDENT` token's recorded column.
* `c_forbidden` is kept as a scanner-side exclusion for reference; the grammar
  itself never references it (matching how the spec states it).
* `s_l_flow_in_block` merges the spec's `s-separate(n+1,FLOW-OUT)` /
  `ns-flow-node(n+1,FLOW-OUT)` into the single `FLOW_OUT` pair because `n+1` is
  a scanner-level column, not a grammar parameter.

### Constraints that stay outside the CFG (semantic / scanner side conditions)

* Implicit keys are limited to 1024 characters (`ns-s-implicit-yaml-key`,
  `c-s-implicit-json-key`) — a length side condition, not a grammar shape.
* The `c-ns-flow-map-separate-value` lookahead `≠ ns-plain-safe(c)` is enforced
  by parse failure: `{a:b}` only derives through the plain-scalar route, since
  the `:` + `e_node` route leaves `b}` unconsumed.
* Tabs may not be used for indentation, and a document's indentation style must
  be consistent — enforced by the `INDENT` token producer.
* Tag resolution / duplicate-key detection (`c-ns-anchor-property` uniqueness)
  is post-parse semantics, outside the CFG as expected for a context-free
  acceptor.

Start symbol: `l_yaml_stream`. No rule in §§1-§9 mentions `c`, `t`, `n` or `m`,
and no rule uses a lookahead/lookbehind assertion, so the document is a
context-free EBNF grammar.
