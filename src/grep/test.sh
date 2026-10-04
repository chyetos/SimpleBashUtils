#!/bin/bash

# Файлы для тестирования
filename="test3.txt"
filename2="test.txt"
patterns_file="patterns.txt"
pattern="hello"

# Функция для проверки без флага -f
check_files() {
    flags="$1"
    echo "Проверка с флагами: ${flags:-без флагов}"
    if [ -z "$flags" ]; then
        # Без флагов
        ./s21_grep "$pattern" "$filename" "$filename2" >> s21_grep.txt
        grep "$pattern" "$filename" "$filename2" >> grep.txt
    else
        # С флагами
        ./s21_grep $flags "$pattern" "$filename" "$filename2" >> s21_grep.txt
        grep $flags "$pattern" "$filename" "$filename2" >> grep.txt
    fi

    if diff -s s21_grep.txt grep.txt; then
        echo -e "\033[1;32mФайлы s21_grep.txt и grep.txt идентичны\033[0m"
    else
        echo -e "\033[1;31mФайлы s21_grep.txt и grep.txt отличаются\033[0m"
    fi
    rm s21_grep.txt grep.txt
}

# Функция для проверки с флагом -f
check_files_f() {
    flags="$1"
    echo "Проверка с флагами (с использованием patterns.txt): ${flags:-без флагов}"
    ./s21_grep $flags -f "$patterns_file" "$filename" "$filename2" >> s21_grep.txt
    grep $flags -f "$patterns_file" "$filename" "$filename2" >> grep.txt

    if diff -s s21_grep.txt grep.txt; then
        echo -e "\033[1;32mФайлы s21_grep.txt и grep.txt идентичны\033[0m"
    else
        echo -e "\033[1;31mФайлы s21_grep.txt и grep.txt отличаются\033[0m"
    fi
    rm s21_grep.txt grep.txt
}



# Список флагов
flags=("-e" "-i" "-v" "-c" "-l" "-n" "-h" "-s" "-o")

# Проверка одиночных флагов (без -f)
echo -e "\033[1;33mПроверка с одиночными флагами (без -f):\033[0m"
for flag in "${flags[@]}"; do
    check_files "$flag"
done

# Проверка комбинаций парных флагов (без -f)
echo -e "\033[1;33mПроверка комбинаций парных флагов (без -f):\033[0m"
for flag1 in "${flags[@]}"; do
    for flag2 in "${flags[@]}"; do
        if [ "$flag1" != "$flag2" ]; then
            check_files "$flag1 $flag2"
        fi
    done
done

# Проверка комбинаций -f с другими флагами
echo -e "\033[1;33mПроверка комбинаций -f с одиночными флагами:\033[0m"
for flag in "${flags[@]}"; do
    check_files_f "$flag"
done

