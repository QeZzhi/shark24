#!/bin/bash

if [ "$EUID" -ne 0 ]; then
  echo -e "\033[1;31m[Ошибка] Запустите скрипт с sudo:\033[0m"
  echo "sudo ./install.sh"
  exit 1
fi

if [ -f "/usr/local/bin/shark24" ]; then
  echo -e "\033[1;33m[Внимание] shark24 уже установлен в /usr/local/bin.\033[0m"
  read -p "Хотите переустановить/обновить его? (y/n): " choice
  case "$choice" in
    y|Y ) echo -e "\033[1;34m[shark24] Продолжаем обновление...\033[0m" ;;
    n|N ) echo -e "\033[1;31m[shark24] Установка отменена.\033[0m"; exit 0 ;;
    * ) echo -e "\033[1;31m[Ошибка] Неверный ввод. Установка отменена.\033[0m"; exit 1 ;;
  esac
else
  echo -e "\033[1;34m[shark24] Начинаем установку...\033[0m"
fi

make clean > /dev/null 2>&1

echo -e "\033[1;34m[shark24] Сборка проекта...\033[0m"
make

if [ $? -ne 0 ]; then
  echo -e "\033[1;31m[Ошибка] Сборка не удалась! Установка прервана.\033[0m"
  exit 1
fi

echo -e "\033[1;34m[shark24] Копирование в /usr/local/bin...\033[0m"
cp shark24 /usr/local/bin/shark24

chmod +x /usr/local/bin/shark24

make clean > /dev/null 2>&1

echo -e "\033[1;32m[shark24] Установка завершена! Shark24 готов к охоте 🦈\033[0m"