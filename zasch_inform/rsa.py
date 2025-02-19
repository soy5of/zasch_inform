import sys

# Увеличиваем лимит для преобразования целых чисел в строки
sys.set_int_max_str_digits(0)  # Устанавливает неограниченный лимит

def read_prime_from_file(filename):
    with open(filename, 'r') as file:
        return int(file.read().strip())

def gcd(a, b):
    while b:
        a, b = b, a % b
    return a

def modinv(a, m):
    m0, x0, x1 = m, 0, 1
    if m == 1:
        return 0
    while a > 1:
        q = a // m
        m, a = a % m, m
        x0, x1 = x1 - q * x0, x0
    if x1 < 0:
        x1 += m0
    return x1

def generate_keys(p, q):
    n = p * q
    phi = (p - 1) * (q - 1)

    e = 65537  # Часто используемое значение для e
    if gcd(e, phi) != 1:
        raise ValueError("e и phi должны быть взаимно простыми")

    d = modinv(e, phi)
    
    return (e, n), (d, n)  # Открытый и закрытый ключи

def encrypt(message, public_key):
    e, n = public_key
    cipher_text = [pow(ord(char), e, n) for char in message]
    return cipher_text

def decrypt(cipher_text, private_key):
    d, n = private_key
    plain_text = ''.join([chr(pow(char, d, n)) for char in cipher_text])
    return plain_text

# Основная программа
if __name__ == "__main__":
    # Считываем простые числа из файлов
    p = read_prime_from_file('prime1.txt')
    q = read_prime_from_file('prime2.txt')

    # Генерируем ключи RSA
    public_key, private_key = generate_keys(p, q)

    print("Открытый ключ:", public_key)
    print("Закрытый ключ:", private_key)

    # Ввод сообщения пользователем
    message = input("Введите сообщение для шифрования: ")
    
    # Шифруем сообщение
    cipher_text = encrypt(message, public_key)
    
    print("Зашифрованное сообщение:", cipher_text)

    # Расшифровываем сообщение
    decrypted_message = decrypt(cipher_text, private_key)
    
    print("Расшифрованное сообщение:", decrypted_message)
