#include <iostream>
#include <string>
using namespace std;

// Khai bao cau truc Chapter
struct Chapter {
    string name_chapter; // Ten chuong hoac muc
    int nums_start;      // So trang bat dau
    int nums_end;        // So trang ket thuc
    Chapter* First_Sec;  // Con tro den muc con dau tien
    Chapter* Section;    // Con tro den muc anh em tiep theo

    // Khoi tao chuong
    Chapter(string name, int start, int end)
        : name_chapter(name), nums_start(start), nums_end(end), First_Sec(nullptr), Section(nullptr) {}
};

// Ham cap nhat so trang bat dau cua chapter
void Update_Chapter_Start_Page(Chapter* root) {
    Chapter* current = root->First_Sec;
    int current_start = root->nums_start;

    // Cap nhat so trang bat dau cua chapter
    while (current) {
        current->nums_start = current_start;
        current_start = current->nums_end + 1;
        current = current->Section;
    }
}

// Ham cap nhat trang cuoi cua chapter
void Update_Chapter_End_Page(Chapter* root) {
    Chapter* current = root->First_Sec;
    while (current) {
        Chapter* section = current->First_Sec;
        int last_section_end = current->nums_end;

        // Tim section cuoi cung trong chapter
        while (section) {
            last_section_end = section->nums_end;
            section = section->Section;
        }

        // Cap nhat trang cuoi cua chapter
        current->nums_end = last_section_end;
        current = current->Section;
    }
}


// Ham kiem tra va dieu chinh so trang
void Check_nums_Chapter(Chapter* root) {
    if (!root) return;

    Chapter* current = root->First_Sec;
    while (current && current->Section) {
        Chapter* next = current->Section;
        // Dieu chinh trang bat dau cua chuong tiep theo
        if (next->nums_start != current->nums_end + 1) {
            next->nums_start = current->nums_end + 1;
            next->nums_end = next->nums_start + (next->nums_end - next->nums_start);
        }
        current = current->Section;
    }

    // Cap nhat so trang cuoi cua chapter sau khi dieu chinh
    Update_Chapter_Start_Page(root);
    Update_Chapter_End_Page(root);
}

// Ham them chuong hoac muc con
void Add_Chapter_or_Section(Chapter* root, string name, int start, int end) {
    if (!root) return;

    Chapter* new_chapter = new Chapter(name, start, end);
    if (!root->First_Sec) {
        root->First_Sec = new_chapter; // Them vao muc con dau tien
    }
    else {
        Chapter* temp = root->First_Sec;
        while (temp->Section) {
            temp = temp->Section; // Duyet den muc cuoi cung
        }
        temp->Section = new_chapter; // Them vao cuoi danh sach muc con
    }

    // Cap nhat so trang khi them chuong/muc con moi
    Check_nums_Chapter(root);
}

// Ham xoa chuong hoac muc con va cap nhat so trang
bool Delete_And_Update(Chapter* root, string name) {
    if (!root || !root->First_Sec) return false;

    Chapter* prev = nullptr;
    Chapter* current = root->First_Sec;

    // Tim chuong/muc con can xoa
    while (current && current->name_chapter != name) {
        prev = current;
        current = current->Section;
    }

    if (!current) return false; // Neu khong tim thay

    // Xoa muc dau danh sach
    if (!prev) {
        root->First_Sec = current->Section;
    }
    else {
        prev->Section = current->Section; // Loai bo muc khoi danh sach
    }

    delete current; // Giai phong bo nho

    // Cap nhat lai so trang cho cac muc sau
    Chapter* updater = prev ? prev->Section : root->First_Sec;
    while (updater) {
        if (prev) {
            updater->nums_start = prev->nums_end + 1;
        }
        else {
            updater->nums_start = root->nums_start + 1;
        }
        updater->nums_end = updater->nums_start + (updater->nums_end - updater->nums_start);
        prev = updater;
        updater = updater->Section;
    }

    // Cap nhat so trang cuoi cua chapter
    Update_Chapter_Start_Page(root);
    Update_Chapter_End_Page(root);
    return true;
}

// Ham in muc luc theo cau truc cay
void Print_TOC(Chapter* root, int level = 0) {
    if (!root) return;

    Chapter* current = root->First_Sec;
    string indent(level * 4, ' '); // Thut dau dong

    while (current) {
        cout << indent << "- " << current->name_chapter
            << " (Trang: " << current->nums_start << " - " << current->nums_end << ")" << endl;
        Print_TOC(current, level + 1); // De quy in muc con
        current = current->Section;
    }
}

// Dem tong so chuong (hoac muc con)
int Count_Chapters(Chapter* root) {
    if (!root) return 0;

    int count = 0;
    Chapter* current = root->First_Sec;
    while (current) {
        count++;
        current = current->Section;
    }
    return count;
}

// Tim chuong dai nhat (so trang nhieu nhat)
Chapter* Find_Longest_Chapter(Chapter* root) {
    if (!root || !root->First_Sec) return nullptr;

    Chapter* longest = nullptr;
    int max_pages = 0;

    Chapter* current = root->First_Sec;
    while (current) {
        int pages = current->nums_end - current->nums_start + 1;
        if (pages > max_pages) {
            max_pages = pages;
            longest = current;
        }
        current = current->Section;
    }

    return longest;
}

// Ham in noi dung chuong dai nhat
void Print_Longest_Chapter(Chapter* root) {
    Chapter* longest = Find_Longest_Chapter(root);
    if (longest) {
        cout << "Chuong dai nhat: " << longest->name_chapter
            << " (Trang: " << longest->nums_start << " - " << longest->nums_end << ")" << endl;
    }
    else {
        cout << "Khong tim thay chuong dai nhat." << endl;
    }
}

int main() {
    // Tao sach moi
    Chapter* book = new Chapter("Root Book", 0, 0);

    // Them cac chuong
    Add_Chapter_or_Section(book, "Chapter 1", 1, 10);
    Add_Chapter_or_Section(book, "Chapter 2", 11, 20);
    Add_Chapter_or_Section(book, "Chapter 3", 21, 30);

    // Them muc con vao Chapter 1
    Add_Chapter_or_Section(book->First_Sec, "Section 1.1", 2, 5);
    Add_Chapter_or_Section(book->First_Sec, "Section 1.2", 6, 9);

    // Them muc con vao Chapter 2
    Add_Chapter_or_Section(book->First_Sec->Section, "Section 2.1", 12, 15);

    // In muc luc truoc khi xoa
    cout << "Muc luc truoc khi xoa:" << endl;
    Print_TOC(book);

    // Xoa Chapter 3
    if (Delete_And_Update(book, "Chapter 3")) {
        cout << "\nSau khi xoa Chapter 3:" << endl;
        Print_TOC(book);
    }

    /* In tong so chuong
    cout << "\nTong so chuong: " << Count_Chapters(book) << endl;

    Tim chuong dai nhat
    Print_Longest_Chapter(book);*/

    //Them mot chuong moi
    Add_Chapter_or_Section(book, "Chapter 4", 21, 35); // Them Chapter 4
    Add_Chapter_or_Section(book->First_Sec->Section->Section, "Section 4.1", 22, 27); // Them Section 4.1 vao Chapter 4
    Add_Chapter_or_Section(book->First_Sec->Section->Section, "Section 4.2", 28, 35); // Them Section 4.2 vao Chapter 4

    // In muc luc sau khi them moi
    cout << "\nMuc luc sau khi them Chapter 4:" << endl;
    Print_TOC(book);

    // In tong so chuong sau khi them
    cout << "\nTong so chuong sau khi them: " << Count_Chapters(book) << endl;

    // Tim chuong dai nhat
    Print_Longest_Chapter(book);

    return 0;
}
