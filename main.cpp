#include "mainwindow.h"
#include <iostream>
#include <assert.h>
#include "dictionary_hash.h"
#include <QApplication>

int isEqual(list<string> queue1, list<string> queue2) {
    if (queue1.size() != queue2.size()) return 0;
    else {
        auto iter2 = queue2.begin();
        for (auto iter1 = queue1.begin(); iter1 != queue1.end(); iter1++) {
            if (*iter2 != *iter1) return 0;
            iter2++;
        }
        return 1;
    }
}

int main(int argc, char *argv[])
{
    //Тест конструктора по умолчанию
    dictionary_hash<string> collection;
    assert(collection.size() == 0);


    //Добавление ключа
    collection.insert("UUU", "Phenylalanine");
    assert(collection.size() == 1);

    //Добавление существующего ключа
    collection.insert("UUU", "Phenylalanine");
    assert(collection.size() == 1);

    //Проверка наличия ключа
    assert(collection.in_dictionary("UUU") == 1);

    //Проверка []
    assert(collection["UUU"] == "Phenylalanine");

    //Проверка удаления
    //Не существует ключ
    collection.delete_word("GUG");
    assert(collection.size() == 1);
    //Существует ключ
    collection.delete_word("UUU");
    assert(collection.size() == 0);

    //Проверка наличия удаленного ключа
    assert(collection.in_dictionary("UUU") == 0);

    //Коллекция после удаления и новая
    dictionary_hash<string> collection2;
    assert(collection == collection2);

    //Конструктор копирования
    dictionary_hash<string> collection_copy(collection);
    assert(collection == collection_copy);
    //Равенство после вставки
    collection_copy.insert("CUA", "Leucine");
    assert(collection != collection_copy);

    //Проверка загрузки из файла и записи в файл
    dictionary_hash<string> collection_out;
    collection_out.insert("UAU", "Tyrosine");
    collection_out.insert("CGA", "Arginine");
    collection_out.insert("AUC", "Isoleucine");
    assert(collection_out.output_to_file("test_out.txt") == 1);
    dictionary_hash<string> collection_in;
    assert(collection_in.input_from_file("test_out.txt") == 1);
    assert(collection_in == collection_out);

    //Проверка удаления всего списка
    assert(collection_in.clear() == 1);
    assert(collection_in.size() == 0);

    //Проверка метода принимающего число
    assert(isEqual(collection_out.count({"UAU","AUC"}),{"Tyrosine","Isoleucine"}));

    cout << "All tests are passed";

    QApplication a(argc, argv);
    MainWindow w;
    w.show();
    return a.exec();
}
