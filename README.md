# Book Catalog

## Overview
This project is used for managing a book collection from the terminal.  
It demonstrates linked‑list data structures, defensive input validation, and modularity using C.  
With a single executable you can **add, view, update, delete, or search** books by title.

Key design points:

* **Singly‑Linked List** – dynamic in‑memory storage with O(1) insertions and O(n) traversals.
* **Robust Validation** – every `scanf/ fgets` is range‑checked; IDs must be positive, publication years ≤ 2025.
* **Menu‑Driven UX** – numeric commands (`1‑5`, `9`) cycle indefinitely until the user exits.
* **Memory‑Safe** – `calloc` for nodes, one‑pass `freeCatalog` before shutdown.
* **Case‑Insensitive Search** – partial‑title matching via `strstr + _strlwr`.


## Feature Table

| Menu # | Action | Workflow Details |
|--------|--------|------------------|
| **1** | Add Book | Prompts for ID, title, author, year. Rejects duplicate IDs and empty fields. |
| **2** | View Books | Prints an aligned table of every node in the list, or warns if empty. |
| **3** | Update Book | Locates by ID, then lets the user overwrite title/author/year in place. |
| **4** | Delete Book | Removes a node by ID (handles head, middle, tail) and frees memory. |
| **5** | Search Books | Case‑insensitive substring search on titles; shows matching rows only. |
| **9** | Exit | Flushes the entire catalog from the heap and says goodbye. |


## Build & Run

Windows User:

1. Download VS Code and C/C++ Extension in VS Code.
2. Download MinGW. Link: https://sourceforge.net/projects/mingw/
3. In Terminal, type gcc Book_Catalog.c -o Book_Catalog.exe for compile the program and build the executable.
4. In Terminal, type .\Book_Catalog.exe to run the program.

Mac User:

1. Open Terminal, type xcode-select --install to install the Command Line Tools.
2. In Terminal, type gcc Book_Catalog.c -o Book_Catalog for compile the prgram and build the executable.
3. Lastly, type ./Book_Catalog to run the program.


## Sample Session
1. addBook
2. viewBooks
3. updateBook
4. deleteBook
5. searchBooks
6. exit program
- Please input a number for operation (only one digit): 1
- Please input Book ID: 100
- Please input Book title: Mistborn
- Please input Author name: Brandon Sanderson
- Please input year book was published: 2006
- Successfully added a Book.

<hr/>

Welcome to clone and fork!

