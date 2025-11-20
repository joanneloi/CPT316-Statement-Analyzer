# CPT316-Statement_Analyzer
## Introduction
This repository contains the code for  a simple Statement Analyzer for the CPT316 Programming Language Implementation and Paradigms at the Universiti Sains Malaysia. The project is part of the coursework for the course.

## Project Description
This project includes a lexer, parser, and parse tree visualization, supporting basic assignment statements of the form: <id> = <expr> ;
The analyzer checks lexical validity, parses input according to the grammar rules, reports syntax errors, and generates a parse tree. 

## COMPY Language Specification
```text
Tokens
├─Identifiers: single-letter, lowercase [a-z]
├─Numbers: integers
├─Operators: +, -, *, /
├─Assignment: =
├─Parentheses: ( )
└─Statement terminator: ;

```text
Grammar
├─<stmt> → id = <expr> ;
├─<expr> → <term> | <expr> ‘+’ <term> | <expr> ‘-’ <term>
├─<term> → <factor> | <term> ‘*’ <factor> | <term> ‘/’ <factor>
└─<factor> → id | int | ( <expr> )

## Team Member
- Joanne Loi Jin XU
- Lim Yi Jie
- Saw Yee Xuan
- Goh Shu Ying
  
## Project Structure
```text
CPT316-Statement-Analyzer-1/
├─ main.cpp             
├─ lexer.h
├─ lexer.cpp
├─ parser.h
├─ parser.cpp
├─ parse_tree.dot        # DOT representation of the parse tree
├─ parse_tree.png        # Rendered parse tree image
└─ README.md
