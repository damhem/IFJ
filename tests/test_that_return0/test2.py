# Program 2: Vypocet faktorialu (rekurzivne)
#navratova hodnota 0
def factorial(n):
    if n < 2:
        result = 1
    else:
# else cast prikazu vetveni, jednoradkovy komentar, negeneruje DEDENT
        decremented_n = n - 1
        temp_result = factorial(decremented_n)
        result = n * temp_result
    return result
# end of factorial function
# Hlavni telo programu
print('Zadejte cislo pro vypocet faktorialu: ')
a = inputi()
if a < 0.0: # pred porovnanim probehne implicitni konverze int na float
    print('Faktorial nelze spocitat')
else:
    vysl = factorial(a)
    print('Vysledek je:', vysl)