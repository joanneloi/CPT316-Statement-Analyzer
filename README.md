# CPT316-Statement-Analyzer


## Features
- **Lexical analysis** – tokenizes identifiers, numbers, operators, parentheses, assignment, and semicolons while rejecting invalid characters, multi-letter identifiers, and alphanumeric mixes such as `a123`.
- **Token reporting** – prints the token stream, absolute positions, and category counts for quick inspections.
- **Syntax analysis** – shift-reduce parser that implements the COMPY expression grammar, enforcing precedence (`*`/`/` over `+`/`-`) and balanced parentheses.
- **Error diagnostics** – distinguishes lexical errors (invalid characters or malformed tokens) from syntax errors (missing operands, parentheses, or terminator).
- **Parse tree export** – writes `parse_tree.dot` so you can render an image with Graphviz (`dot -Tpng parse_tree.dot -o parse_tree.png`).

## COMPY Language Specification
```text
Tokens
├─ Identifiers: single lowercase letter [a-z]
├─ Numbers: integer literals
├─ Operators: +  -  *  /
├─ Assignment: =
├─ Parentheses: ( )
└─ Terminator: ;
```

```text
Grammar
├─ <stmt>   → id = <expr> ;
├─ <expr>   → <term>
│            | <expr> + <term>
│            | <expr> - <term>
├─ <term>   → <factor>
│            | <term> * <factor>
│            | <term> / <factor>
└─ <factor> → id | int | ( <expr> )
```

## Repository Layout
```text
CPT316-Statement-Analyzer/
├─ main.cpp          # Entry point and CLI runner
├─ lexer.h/.cpp      # Token model, lexer, statistics, and diagnostics
├─ parser.h/.cpp     # Shift-reduce parser and parse-tree emitter
├─ parse_tree.dot    # Sample DOT output (overwritten on each run)
├─ parse_tree.png    # Example rendered tree (not regenerated automatically)
└─ README.md
```

## Component Responsibilities
- `main.cpp`: orchestrates a single input run, invokes the lexer first, reports tokens and counts, then hands the token sequence to the parser if no lexical errors exist.
- `lexer.h/.cpp`: defines `Token`, `TokenCounts`, and `LexicalError`, scans raw text into tokens, tallies categories, and logs invalid characters or malformed identifiers/numbers.
- `parser.h/.cpp`: implements a shift-reduce parser with operator-precedence safeguards, produces parse errors with positions, and writes the parse tree to DOT format when the statement is accepted.
- `parse_tree.dot` / `parse_tree.png`: artifacts of the latest successful parse, useful for visual inspection of the derivation tree.

## Build and Run
```bash
g++ -std=c++17 -O2 -o statement_analyzer main.cpp lexer.cpp parser.cpp
./statement_analyzer
```
On Windows PowerShell:
```powershell
g++ -std=c++17 -O2 -o statement_analyzer.exe main.cpp lexer.cpp parser.cpp
.\statement_analyzer.exe
```

## Usage Flow
1. Launch the executable; you will be prompted for a single COMPY statement such as `x = (3 + 5) * 2;`
2. The program prints:
   - Token table and positions.
   - Aggregate counts per token category.
   - Lexical errors, if any (parsing stops when they exist).
   - Parsing result (accepted/rejected) and detailed syntax errors.
3. On acceptance, `parse_tree.dot` is emitted. Convert it to PNG via `dot -Tpng parse_tree.dot -o parse_tree.png`.

### Sample Session
```
Enter a single-line COMPY statement (e.g., x = (3 + 5) * 2;):

x = (3 + 5) * 2;
```
The analyzer prints the tokens, reports zero lexical errors, and shows `Input accepted.` followed by DOT generation instructions.

## Error Reporting
- **Lexical layer**: flags invalid identifiers, illegal characters, or number/letter mixes. When lexical errors exist the parser stage is skipped.
- **Syntax layer**: detects missing operands around operators, mismatched parentheses, and missing semicolons or identifiers. Each error is annotated with the token position to simplify debugging.

## Parse Tree Visualization
The parser builds an explicit tree of grammar reductions. After a successful parse:
1. Inspect `parse_tree.dot`.
2. Render using Graphviz:
   ```
   dot -Tpng parse_tree.dot -o parse_tree.png
   ```
3. Open `parse_tree.png` to verify the derivation.

## Extending the Analyzer
- Add new tokens by extending `TokenType` plus the scanning logic in `lexer.cpp`.
- Update the grammar rules in `parser.cpp::tryReduce` to support new constructs.
- Enhance diagnostics by enriching `checkError` or emitting additional context in `printParseErrors`.

The project purposefully keeps every stage isolated lexer, parser, visualization
