""" Program 3: Prace s retezci a vestavenymi funkcemi """
##navratova hodnota 0
s1 = 'Toto je nejaky text'
s2 = s1 + ', ktery jeste trochu obohatime'
print(s1, '\n', s2)
s1len = len(s1)
s1len = s1len - 4
s1 = substr(s2, s1len, 4)
s1len = s1len + 1 # korekce indexu znaku na poradi znaku
print('4 znaky od ', s1len, '. znaku v "', s2, '\":', s1)
print('Zadejte serazenou posloupnost vsech malych pismen a-h, ')
print('pricemz se pismena nesmeji v posloupnosti opakovat: ')
s1 = inputs()
if s1 != None:
    while s1 != 'abcdefgh':
        print('Spatne zadana posloupnost, zkuste znovu: ')
        s1 = inputs()
else: # nezadan zadny vstup (EOF)
    pass
