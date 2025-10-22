# class Parser:
#     def __init__(self, tokens):
#         self.tokens = tokens
#         self.pos = 0

#     def current(self):
#         if self.pos < len(self.tokens):
#             return self.tokens[self.pos]
#         return None

#     def eat(self, expected):
#         if self.current() == expected:
#             self.pos += 1
#         else:
#             raise SyntaxError(f"Expected '{expected}' but got '{self.current()}'")

#     def parse(self):
#         self.stmt()
#         if self.current() is not None:
#             raise SyntaxError("Extra tokens after valid statement")
#         print("✅ Valid statement!")

#     # <stmt> → id = <expr> ;
#     def stmt(self):
#         self.eat("id")
#         self.eat("op")  # '=' symbol represented as 'op'
#         self.expr()
#         self.eat("semi")

#     # <expr> → <term> | <expr> + <term> | <expr> - <term>
#     def expr(self):
#         self.term()
#         while self.current() == "op":
#             # Look ahead to check if the op is '+' or '-'
#             # Here you may have to check actual lexeme if available
#             self.eat("op")
#             self.term()

#     # <term> → <factor> | <term> * <factor> | <term> / <factor>
#     def term(self):
#         self.factor()
#         while self.current() == "op":
#             # same idea — could be '*' or '/'
#             self.eat("op")
#             self.factor()

#     # <factor> → id | int | ( <expr> )
#     def factor(self):
#         if self.current() in ["id", "int"]:
#             self.eat(self.current())
#         elif self.current() == "lparen":
#             self.eat("lparen")
#             self.expr()
#             self.eat("rparen")
#         else:
#             raise SyntaxError(f"Unexpected token '{self.current()}'")

# def main():
#     token_input = "id,op,id,op,int,semi"
#     tokens = token_input.split(",")
#     parser = Parser(tokens)
#     parser.parse()

# if __name__ == "__main__":
#     main()


# syntax.py

class Parser:
    def __init__(self, tokens):
        self.tokens = tokens
        print("Tokens:", self.tokens)  # Check tokens
        self.pos = 0

    def current_token(self):
        return self.tokens[self.pos] if self.pos < len(self.tokens) else None

    def match(self, expected):
        if self.current_token() == expected:
            self.pos += 1
            return True
        else:
            raise SyntaxError(f"Expected '{expected}' but found '{self.current_token()}'")

    # Grammar rules
    def stmt(self):
        node = {"stmt": []}
        node["stmt"].append(self.match_token("id"))
        node["stmt"].append(self.assign())
        node["stmt"].append(self.expr())
        node["stmt"].append(self.match_token("semi"))
        return node

    def expr(self):
        node = {"expr": []}
        node["expr"].append(self.match_token("id"))
        node["expr"].append(self.match_token("op"))
        node["expr"].append(self.match_token("int"))
        return node

    def assign(self):
        return self.match_token("assign")

    def match_token(self, expected):
        if self.current_token() == expected:
            token = self.current_token()
            self.pos += 1
            return token
        else:
            raise SyntaxError(f"Syntax error: expected {expected}, got {self.current_token()}")

    def parse(self):
        tree = self.stmt()
        if self.pos < len(self.tokens):
            raise SyntaxError("Extra tokens after valid statement")
        return tree


def print_tree(node, indent=0):
    if isinstance(node, dict):
        for key, value in node.items():
            print("  " * indent + key)
            for child in value:
                print_tree(child, indent + 1)
    else:
        print("  " * indent + str(node))


def main():
    token_input = "id,assign,id,op,int,semi"
    tokens = token_input.split(",")
    parser = Parser(tokens)
    tree = parser.parse()
    print("✅ Syntax Tree:")
    print_tree(tree)


if __name__ == "__main__":
    main()
