#include <stdio.h>
#include <gmp.h>
#include <stdlib.h>
#include <time.h>

// Функция для генерации случайного простого числа указанной длины
void generate_random_prime(int bits, const char *filename, unsigned int seed) {
    FILE *file = fopen(filename, "w"); // Открытие файла для записи
    if (!file) {
        perror("Failure opening file");
        return;
    }

    mpz_t random_number;
    gmp_randstate_t state;

    mpz_init(random_number); // Инициализация переменной для хранения случайного числа

    gmp_randinit_default(state); // Инициализация генератора случайных чисел
    gmp_randseed_ui(state, seed); // Использование переданного seed

    do {
        // Генерация случайного числа указанной длины в битах
        mpz_urandomb(random_number, state, bits);

        // Проверка, является ли число простым
    } while (mpz_probab_prime_p(random_number, 50) == 0); // 50 итераций теста Миллера-Рабина

    // Запись простого числа в файл
    gmp_fprintf(file, "%Zd\n", random_number);

    // Вывод на экран
    gmp_printf("Random prime number (%d bits): %Zd\n", bits, random_number);

    fclose(file); // Закрытие файла
    mpz_clear(random_number); // Освобождение памяти
    gmp_randclear(state);
}

// Функция для приостановки выполнения на заданное количество микросекунд
void sleep_microseconds(long microseconds) {
    struct timespec ts;
    ts.tv_sec = microseconds / 1000000; // Переводим в секунды
    ts.tv_nsec = (microseconds % 1000000) * 1000; // Остаток в наносекундах

    nanosleep(&ts, NULL); // Приостановка выполнения
}

int main() {
    int bits = 65536; // Длина генерируемого простого числа в битах. У меня всё хорошо работало до 4096, дальше - хуже, но тоже должно.

    // Разные seed для каждого вызова
    unsigned int seed1 = time(NULL);
    sleep_microseconds(1); // Ждем 1 сек, чтобы time(NULL) изменилось

    unsigned int seed2 = time(NULL);
    while (seed2 == seed1) { // Если время совпало ждем еще
        sleep_microseconds(1);
        seed2 = time(NULL);
    }

    generate_random_prime(bits, "prime1.txt", seed1);
    generate_random_prime(bits, "prime2.txt", seed2);

    return 0;
}
