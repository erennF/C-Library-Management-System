# C-Library-Management-System
A comprehensive Library Management System developed as part of my Software Engineering studies. This project demonstrates core programming concepts such as role-based access control, persistent data storage through file handling, and modular system design.

🚀 Key Features
Role-Based Access Control: Separate interfaces and functionalities for Admin and User roles.
Persistent Data Storage: Efficiently manages data using structured .txt files for users, books, loans, and system messages.
Admin Capabilities: Full CRUD operations for managing the book collection and user database, plus system notification tools for overdue books.
User Functionalities: Advanced search options (by title, author, or category), book borrowing/returning workflows, and personal notification inbox.
Automated Notifications: Built-in logic to track loan periods and alert users when the 15-day limit is exceeded.

🛠 Technical Specifications
Language: C
Data Structures: Custom struct definitions for complex data modeling.
File I/O: Robust file handling to ensure data integrity across sessions.
Time Management: Integration with <time.h> for real-time loan tracking and deadline calculations.

📂 File Structure
main.c: The core logic and entry point of the application.
books.txt / users.txt: Databases for library inventory and registered members.
loans.txt: Dynamic log tracking all active and past book borrowings.
messages.txt: System-generated alerts and user notifications.
