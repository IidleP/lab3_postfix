#include "queue.h"
#include <gtest.h>

// 1. Создание очереди
TEST(TQueue, can_create_queue_with_positive_size)
{
    ASSERT_NO_THROW(TQueue<int> q(5));
}

// 2. Создание с нулевым размером - ошибка
TEST(TQueue, throws_when_create_queue_with_zero_size)
{
    ASSERT_ANY_THROW(TQueue<int> q(0));
}

// 3. Создание с отрицательным размером - ошибка
TEST(TQueue, throws_when_create_queue_with_negative_size)
{
    ASSERT_ANY_THROW(TQueue<int> q(-5));
}

// 4. Создание с размером больше максимума - ошибка
TEST(TQueue, throws_when_create_queue_with_too_large_size)
{
    ASSERT_ANY_THROW(TQueue<int> q(MaxQueueSize + 1));
}

// 5. Новая очередь пуста
TEST(TQueue, new_queue_is_empty)
{
    TQueue<int> q(5);
    EXPECT_TRUE(q.isEmpty());
    EXPECT_EQ(0, q.GetSize());
}

// 6. Очередь становится полной
TEST(TQueue, queue_is_full_when_reaches_capacity)
{
    TQueue<int> q(3);
    q.Push(1);
    q.Push(2);
    q.Push(3);

    EXPECT_TRUE(q.isFull());
    EXPECT_EQ(3, q.GetSize());
}

// 7. Добавление элемента
TEST(TQueue, can_push_element)
{
    TQueue<int> q(3);
    ASSERT_NO_THROW(q.Push(10));
    EXPECT_EQ(1, q.GetSize());
}

// 8. Извлечение элемента (FIFO)
TEST(TQueue, pop_returns_first_pushed_element)
{
    TQueue<int> q(5);
    q.Push(1);
    q.Push(2);
    q.Push(3);

    EXPECT_EQ(1, q.Pop());  // Первым пришел - первым ушел
    EXPECT_EQ(2, q.Pop());
    EXPECT_EQ(3, q.Pop());
}

// 9. Ошибка при извлечении из пустой очереди
TEST(TQueue, throws_when_pop_from_empty_queue)
{
    TQueue<int> q(5);
    ASSERT_ANY_THROW(q.Pop());
}

// 10. Ошибка при добавлении в полную очередь
TEST(TQueue, throws_when_push_to_full_queue)
{
    TQueue<int> q(2);
    q.Push(1);
    q.Push(2);
    ASSERT_ANY_THROW(q.Push(3));
}

// 11. Просмотр первого элемента без удаления
TEST(TQueue, front_returns_first_element_without_removing)
{
    TQueue<int> q(3);
    q.Push(4);
    q.Push(9);

    EXPECT_EQ(4, q.Front());  // Первый элемент
    EXPECT_EQ(2, q.GetSize());  // Размер не изменился
}

// 12. Ошибка при просмотре пустой очереди
TEST(TQueue, throws_when_front_from_empty_queue)
{
    TQueue<int> q(3);
    ASSERT_ANY_THROW(q.Front());
}

// 13. Круговой буфер работает
TEST(TQueue, circular_buffer_works_correctly)
{
    TQueue<int> q(3);
    q.Push(1);
    q.Push(2);
    q.Push(3);
    q.Pop();  // Освобождаем место

    ASSERT_NO_THROW(q.Push(4));  // Используем освободившееся место
    EXPECT_EQ(3, q.GetSize());
}

// 14. Копирование очереди
TEST(TQueue, copied_queue_is_equal_to_source)
{
    TQueue<int> q(3);
    q.Push(1);
    q.Push(2);
    q.Push(3);

    TQueue<int> copy(q);

    EXPECT_EQ(3, copy.GetSize());
    EXPECT_EQ(1, copy.Pop());
    EXPECT_EQ(2, copy.Pop());
    EXPECT_EQ(3, copy.Pop());
}

// 15. Копия имеет свою собственную память
TEST(TQueue, copied_queue_has_its_own_memory)
{
    TQueue<int> q(3);
    q.Push(1);
    q.Push(2);

    TQueue<int> copy(q);
    copy.Pop();

    EXPECT_EQ(2, q.GetSize());  // Оригинал не изменился
    EXPECT_EQ(1, copy.GetSize());
}

// 16. Присваивание очередей
TEST(TQueue, can_assign_queues)
{
    TQueue<int> a(3);
    TQueue<int> b(5);

    a.Push(7);
    a.Push(8);
    b = a;

    EXPECT_EQ(2, b.GetSize());
    EXPECT_EQ(7, b.Pop());
    EXPECT_EQ(8, b.Pop());
}

// 17. Присвоенная очередь имеет свою память
TEST(TQueue, assigned_queue_has_its_own_memory)
{
    TQueue<int> a(3);
    TQueue<int> b(3);

    a.Push(1);
    a.Push(2);
    b = a;
    b.Pop();

    EXPECT_EQ(2, a.GetSize());  // Оригинал не изменился
    EXPECT_EQ(1, b.GetSize());
}

// 18. Можно присвоить очередь самой себе
TEST(TQueue, can_assign_queue_to_itself)
{
    TQueue<int> q(3);
    q.Push(1);
    q.Push(2);

    ASSERT_NO_THROW(q = q);
    EXPECT_EQ(2, q.GetSize());
    EXPECT_EQ(1, q.Front());
}

// 19. Конструктор по умолчанию
TEST(TQueue, can_create_queue_with_default_constructor)
{
    TQueue<int> q;  // Объявляем переменную здесь, а не внутри ASSERT_NO_THROW
    ASSERT_NO_THROW(TQueue<int> q2);  // Проверяем, что можно создать
    EXPECT_TRUE(q.isEmpty());  // Проверяем созданный выше объект
}

// 20. GetMaxSize возвращает правильное значение
TEST(TQueue, get_max_size_returns_correct_value)
{
    TQueue<int> q(7);
    EXPECT_EQ(7, q.GetMaxSize());
}