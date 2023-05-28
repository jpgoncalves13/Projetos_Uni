from lex_parser import lexer

data = """
- var friends = 10
case friends
  when 0
    p you have no friends
  when 1
    p you have a friend
  default
    p you have #{friends} friends
"""

lexer.input(data)

for tok in lexer:
    print(tok)