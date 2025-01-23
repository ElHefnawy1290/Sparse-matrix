#include <iostream>
#include <cassert>
using namespace std;

/*
  Node representing a non-zero column entry in a sparse matrix row.
 */
struct ColumnNode
{
    int data{};    // Value of the matrix element
    int columns{}; // Column index of the element
    ColumnNode *next{};
    ColumnNode *prev{};
    ColumnNode(int data, int columns) : data(data), columns(columns) {}
};

/*
  Doubly linked list representing a sparse row with non-zero elements stored in sorted order.
 */
class ColumnLinkedList
{
private:
    ColumnNode *head{};
    ColumnNode *tail{};
    int length{};
    int columns{}; // Total number of columns in the matrix

    // Utility function to link two nodes together
    void link(ColumnNode *first, ColumnNode *second)
    {
        if (first)
            first->next = second;
        if (second)
            second->prev = first;
    }

    // Inserts a node while maintaining sorted order
    void insert_sorted(ColumnNode *node)
    {
        if (!head || head->columns > node->columns)
        {
            insert_front(node);
            return;
        }
        if (head->columns == node->columns)
        {
            head->data = node->data;
            return;
        }
        ColumnNode *curr = head;
        for (; curr->next; curr = curr->next)
        {
            if (curr->next->columns == node->columns)
            {
                curr->next->data = node->data; // Update existing value
                return;
            }
            if (curr->next->columns > node->columns)
                break;
        }
        bool is_tail = false;
        if (!curr->next)
            is_tail = true;
        link(node, curr->next);
        link(curr, node);
        if (is_tail)
            tail = node;
        length++;
    }
    // Inserts a node at the front
    void insert_front(ColumnNode *node)
    {
        link(node, head);
        if (!head)
            head = tail = node;
        else
            head = node;
        length++;
    }

public:
    ColumnLinkedList() {}
    ColumnLinkedList(int columns) : columns(columns) {}
    ~ColumnLinkedList()
    {
        ColumnNode *curr = head;
        while (curr)
        {
            ColumnNode *temp = curr;
            curr = curr->next;
            delete temp;
        }
        head = tail = nullptr;
    }

    // Set a value in the column-linked list
    void set_value(int data, int column)
    {
        ColumnNode *temp = new ColumnNode(data, column);
        insert_sorted(temp);
    }

    // Get the value at a specific column index
    int get_value(int column) const
    {
        assert(column < columns);
        ColumnNode *curr = head;
        while (curr)
        {
            if (curr->columns == column)
                return curr->data;
            curr = curr->next;
        }
        return 0; // Return 0 if element is not explicitly stored
    }

    // Print the entire row including zeroes
    void print_array() const
    {
        int count = 0;
        ColumnNode *curr = head;
        while (count != columns)
        {
            if (curr && curr->columns == count)
            {
                cout << curr->data << " ";
                curr = curr->next;
            }
            else
                cout << 0 << " ";
            count++;
        }
        cout << "\n";
    }

    // Print only non-zero values in the row
    void print_array_nonzero() const
    {
        ColumnNode *curr = head;
        while (curr)
        {
            cout << curr->data << " ";
            curr = curr->next;
        }
        cout << "\n";
    }

    // Add another sparse row to the current row
    void add(ColumnLinkedList &other)
    {
        ColumnNode *curr = other.head;
        while (curr)
        {
            ColumnNode *curr2 = head;
            bool existed = false;
            while (curr2)
            {
                if (curr2->columns == curr->columns)
                {
                    curr2->data += curr->data;
                    existed = true;
                }
                curr2 = curr2->next;
            }
            if (!existed)
            {
                ColumnNode *temp = new ColumnNode(curr->data, curr->columns);
                insert_sorted(temp);
            }
            curr = curr->next;
        }
    }
};

/**
  Node representing a non-zero row in a sparse matrix.
 */
struct rowNode
{
    int row{};             // Row index
    ColumnLinkedList list; // Sparse row representation
    rowNode *next{};
    rowNode *prev{};
    rowNode(int row, int columns) : row(row), list(columns) {}
};

/**
  Sparse matrix represented as a doubly linked list of non-zero rows.
 */
class SparseMatrix
{
private:
    rowNode *head{};
    rowNode *tail{};
    int length{};
    int rows{};
    int columns{};
    void link(rowNode *first, rowNode *second)
    {
        if (first)
            first->next = second;
        if (second)
            second->prev = first;
    }
    void insert_sorted(int data, rowNode *node, int column)
    {
        if (!head || head->row > node->row)
        {
            insert_front(node, data, column);
            return;
        }
        if (head->row == node->row)
        {
            head->list.set_value(data, column);
            return;
        }
        rowNode *curr = head;
        for (; curr->next; curr = curr->next)
        {
            if (curr->next->row == node->row)
            {
                curr->next->list.set_value(data, column);
                return;
            }
            if (curr->next->row > node->row)
                break;
        }
        bool is_tail = false;
        if (!curr->next)
            is_tail = true;
        link(node, curr->next);
        link(curr, node);
        node->list.set_value(data, column);
        if (is_tail)
            tail = node;
        length++;
    }

    void insert_front(rowNode *node, int data, int column)
    {
        link(node, head);
        if (!head)
            head = tail = node;
        else
            head = node;
        node->list.set_value(data, column);
        length++;
    }

public:
    SparseMatrix(int rows, int columns) : rows(rows), columns(columns) {}
    ~SparseMatrix()
    {
        rowNode *curr = head;
        while (curr)
        {
            rowNode *temp = curr;
            curr = curr->next;
            delete temp; // this will automatically calls column's destructor
        }
        head = tail = nullptr;
    }
    void set_value(int data, int row, int column)
    {
        rowNode *temp = new rowNode(row, columns);
        insert_sorted(data, temp, column);
    }

    int get_value(int row, int column) const
    {
        assert(row < length);
        rowNode *curr = head;
        while (curr)
        {
            if (curr->row == row)
                return curr->list.get_value(column);
            curr = curr->next;
        }
        return 0;
    }
    void print_matrix() const
    {
        int count = 0;
        rowNode *curr = head;
        while (count != rows)
        {
            if (curr && curr->row == count)
            {
                curr->list.print_array();
                curr = curr->next;
            }
            else
            {
                int cnt = 0;
                while (cnt++ != columns)
                    cout << 0 << " ";
                cout << "\n";
            }
            count++;
        }
        cout << "\n";
    }

    void print_matrix_nonzero() const
    {
        rowNode *curr = head;
        while (curr)
        {
            curr->list.print_array_nonzero();
            curr = curr->next;
        }
        cout << "\n";
    }

    void add(SparseMatrix &other)
    {
        rowNode *curr = other.head;
        while (curr)
        {
            rowNode *curr2 = head;
            bool existed = false;
            while (curr2)
            {
                if (curr2->row == curr->row)
                {
                    curr2->list.add(curr->list);
                    existed = true;
                }
                curr2 = curr2->next;
            }
            if (!existed)
            {
                rowNode *temp = new rowNode(curr->row, columns);
                if (!head || head->row > curr->row)
                {
                    if (!head)
                        head = tail = temp;
                    else
                    {
                        link(temp, head);
                        head = temp;
                    }
                    head->list.add(curr->list);
                    curr = curr->next;
                    continue;
                }
                auto cur = head;
                for (; cur->next; cur = cur->next)
                {
                    if (cur->next->row > temp->row)
                        break;
                }
                bool is_tail = false;
                if (!cur->next)
                    is_tail = true;
                link(temp, cur->next);
                link(cur, temp);
                if (is_tail)
                    tail = cur->next;
                cur->next->list.add(curr->list);
            }
            curr = curr->next;
        }
    }
};