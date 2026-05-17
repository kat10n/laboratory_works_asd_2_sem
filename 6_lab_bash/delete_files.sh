#!/bin/bash

if [ $# -ne 2 ]; then
    echo "Использование: $0 <суффикс> <дней>"
    echo "Пример: $0 .log 30"
    exit 1
fi

SUFFIX=$1
DAYS=$2

if ! [[ "$DAYS" =~ ^[0-9]+$ ]]; then
    echo "Ошибка: количество дней должно быть целым числом"
    exit 1
fi

echo "Ищем файлы *${SUFFIX}, к которым не обращались более ${DAYS} дней..."
echo ""

FILES=$(find . -name "*${SUFFIX}" -atime +${DAYS} -type f)

if [ -z "$FILES" ]; then
    echo "Файлов не найдено."
    exit 0
fi

echo "Найдены следующие файлы:"
echo "$FILES"
echo ""

read -p "Удалить все эти файлы? (y/n): " CONFIRM

if [ "$CONFIRM" != "y" ]; then
    echo "Отменено."
    exit 0
fi

COUNT=0
while IFS= read -r FILE; do
    rm "$FILE"
    echo "Удалён: $FILE"
    COUNT=$((COUNT + 1))
done <<< "$FILES"

echo ""
echo "Готово. Удалено файлов: ${COUNT}"
