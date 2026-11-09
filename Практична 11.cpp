#include <iostream>
#include <string>
#include <sstream>
using namespace std;

// Завдання 1. Структура вузла
struct Node {
    int number = 0;
    string direction = "";
    string time = "";
    Node* next = nullptr;
};

struct List {
    Node* head = nullptr;
    Node* tail = nullptr;
    int count = 0;
};

int created = 0, freed = 0;

// Створення вузла
Node* createNode(int n) {
    Node* p = new Node;
    p->number = n;
    p->direction = "Маршрут " + to_string(n);
    p->time = "12:00";
    created++;
    return p;
}

// Завдання 3. Додавання в кінець
void addEnd(List& l, int n) {
    Node* p = createNode(n);

    if (!l.head)
        l.head = p;
    else
        l.tail->next = p;

    l.tail = p;
    l.count++;
}

// Завдання 4. Додавання на початок
void addStart(List& l, int n) {
    Node* p = createNode(n);
    p->next = l.head;
    l.head = p;

    if (!l.tail)
        l.tail = p;

    l.count++;
}

// Завдання 5. Перегляд списку
string view(const List& l, bool print = true) {
    ostringstream out;
    Node* p = l.head;
    int i = 0;

    if (!p) out << "Список порожній\n";
    else out << "№ | Рейс | Напрямок | Час\n";

    while (p && i < l.count) {
        out << ++i << " | " << p->number
            << " | " << p->direction
            << " | " << p->time << '\n';
        p = p->next;
    }

    out << "Переглянуто вузлів: " << i << '\n';

    if (print) cout << out.str();
    return out.str();
}

// Завдання 6. Перегляд адрес
void addresses(const List& l) {
    Node* p = l.head;
    int i = 0;

    while (p && i < l.count) {
        cout << "Вузол " << ++i
            << " | Адреса: " << p
            << " | Наступний: ";

        if (p->next) cout << p->next;
        else cout << "немає";

        cout << '\n';
        p = p->next;
    }
}

// Завдання 7. Перевірка структури
bool check(const List& l, bool print = false) {
    bool ok = true;

    if (l.count < 0 ||
        (l.count == 0 && (l.head || l.tail)) ||
        (l.count > 0 && (!l.head || !l.tail))) {
        if (print) cout << "Помилка порожнього стану\n";
        ok = false;
    }

    // Перевірка циклу
    Node* slow = l.head;
    Node* fast = l.head;

    while (fast && fast->next) {
        slow = slow->next;
        fast = fast->next->next;

        if (slow == fast) {
            if (print) cout << "Виявлено можливий цикл\n";
            return false;
        }
    }

    Node* p = l.head;
    Node* last = nullptr;
    int actual = 0;

    while (p && actual <= l.count) {
        last = p;
        p = p->next;
        actual++;
    }

    if (actual != l.count) {
        if (print) cout << "Помилка кількості або розрив зв'язку\n";
        ok = false;
    }

    if (last != l.tail || (l.tail && l.tail->next)) {
        if (print) cout << "Неправильний покажчик кінця\n";
        ok = false;
    }

    if (print)
        cout << (ok ? "Структура правильна\n"
            : "Структура неправильна\n");

    return ok;
}

// Завдання 8. Очищення
void clear(List& l) {
    Node* p = l.head;

    while (p) {
        Node* next = p->next;
        delete p;
        freed++;
        p = next;
    }

    l.head = nullptr;
    l.tail = nullptr;
    l.count = 0;
}

// Перевірка одного тесту
bool test(const int a[], int n, bool reverse) {
    List l;

    for (int i = 0; i < n; i++) {
        if (reverse) addStart(l, a[i]);
        else addEnd(l, a[i]);
    }

    bool ok = check(l);
    Node* p = l.head;

    for (int i = 0; i < n; i++) {
        int expected = reverse ? a[n - 1 - i] : a[i];

        if (!p || p->number != expected) {
            ok = false;
            break;
        }
        p = p->next;
    }

    if (p) ok = false;

    clear(l);
    return ok && check(l) && created == freed;
}

// 15 автоматизованих тестів
void runTests() {
    int one[] = { 7 };
    int a[] = { 1, 2, 3 };
    int b[] = { 5, 5, 5 };
    int c[] = { -2, 0, 4 };
    int ten[10], hundred[100];

    for (int i = 0; i < 10; i++) ten[i] = i + 1;
    for (int i = 0; i < 100; i++) hundred[i] = i + 1;

    bool r[15];

    r[0] = test(one, 0, false);
    r[1] = test(one, 1, false);
    r[2] = test(one, 1, true);
    r[3] = test(a, 3, false);
    r[4] = test(a, 3, true);
    r[5] = test(b, 3, false);
    r[6] = test(c, 3, false);
    r[7] = test(c, 3, true);
    r[8] = test(ten, 10, false);
    r[9] = test(ten, 10, true);

    // Тест 11. Два однакові перегляди
    List l;
    for (int x : a) addEnd(l, x);

    Node* first = l.head;
    Node* last = l.tail;
    string v1 = view(l, false);
    string v2 = view(l, false);

    r[10] = v1 == v2 && check(l) &&
        l.head == first && l.tail == last;
    clear(l);
    r[10] = r[10] && created == freed;

    // Тест 12. Сто елементів
    r[11] = test(hundred, 100, false);

    // Тест 13. Повторне створення
    for (int x : a) addEnd(l, x);
    clear(l);
    bool empty = check(l);
    for (int x : b) addStart(l, x);
    r[12] = empty && check(l) && l.head->number == 5;
    clear(l);
    r[12] = r[12] && created == freed;

    // Тест 14. Два цикли
    r[13] = test(a, 3, false) && test(c, 3, true);

    // Тест 15. Діагностика зв'язків
    for (int x : a) addEnd(l, x);
    Node* p = l.head;
    int links = 0;

    while (p && links < l.count) {
        links++;
        p = p->next;
    }

    r[14] = links == 3 && p == nullptr && check(l);
    clear(l);
    r[14] = r[14] && created == freed;

    int passed = 0;

    for (int i = 0; i < 15; i++) {
        cout << "Тест " << i + 1 << ": "
            << (r[i] ? "ПРОЙДЕНО" : "ПОМИЛКА") << '\n';
        if (r[i]) passed++;
    }

    cout << "Успішних тестів: " << passed << "/15\n";
}

// Експеримент для 10, 100, 1000 вузлів
void experiment(int n) {
    List l;
    int before = created;

    for (int i = 1; i <= n; i++)
        addEnd(l, i);

    bool ok = check(l);
    int added = created - before;

    clear(l);

    cout << "Вузлів: " << n
        << " | Створено: " << added
        << " | Перевірка: " << (ok ? "успішно" : "помилка")
        << " | Активних: " << created - freed << '\n';
}

int main() {
    List l;

    cout << "\n=== Завдання 1. Структура вузла ===\n";
    Node p;
    cout << "Номер: " << p.number
        << " | Наступний: "
        << (p.next == nullptr ? "немає" : "є") << '\n';

    cout << "\n=== Завдання 2. Порожній список ===\n";
    check(l, true);

    cout << "\n=== Завдання 3. Додавання в кінець ===\n";
    addEnd(l, 101);
    addEnd(l, 202);
    addEnd(l, 303);
    view(l);

    cout << "\n=== Завдання 4. Додавання на початок ===\n";
    clear(l);
    addStart(l, 101);
    addStart(l, 202);
    addStart(l, 303);
    view(l);

    cout << "\n=== Завдання 5. Повторний перегляд ===\n";
    view(l);
    view(l);

    cout << "\n=== Завдання 6. Адреси та зв'язки ===\n";
    addresses(l);

    cout << "\n=== Завдання 7. Перевірка структури ===\n";
    check(l, true);

    cout << "\n=== Завдання 8. Очищення списку ===\n";
    clear(l);
    check(l, true);
    cout << "Активних вузлів: " << created - freed << '\n';

    cout << "\n=== Автоматизовані тести ===\n";
    runTests();

    cout << "\n=== Експеримент ===\n";
    experiment(10);
    experiment(100);
    experiment(1000);

    cout << "\n=== Перевірка пам'яті ===\n";
    cout << "Створено: " << created << '\n';
    cout << "Звільнено: " << freed << '\n';
    cout << "Активних вузлів: " << created - freed << '\n';

    return 0;
}
