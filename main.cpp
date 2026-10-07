#include <QCoreApplication>
#include <QTextStream>
#include <QStringList>
#include <QFile>
#include "calculator.h"
    // Function for displaying the list of available commands
    void printHelp(QTextStream& out) {
    out << "Available commands:\n";
    out << " add <a> <b> - addition\n";
    out << " sub <a> <b> - subtraction\n";
    out << " mul <a> <b> - multiplication\n";
    out << " div <a> <b> - division\n";
    out << " reset - reset calculator\n";
    out << " help - show this help\n";
    out << " quit - exit\n";
}
int main(int argc, char* argv[]) {
    // QCoreApplication is used instead of QApplication for a console application
    QCoreApplication app(argc, argv);
    // Input/output streams
    QTextStream in(stdin);
    QTextStream out(stdout);
    // Create the calculator
    // No parent is needed because it lives until the end of the program
    Calculator calc;
    QFile historyFile("history.txt");
    historyFile.open(QIODevice::WriteOnly | QIODevice::Append);
    QTextStream historyStream(&historyFile);
    QObject::connect(&calc, &Calculator::resultReady,
                     [&historyStream](double result) {
                         historyStream << "Result: " << result << "\n";
                         historyStream.flush();
                     });
    // CONNECTIONS: connect calculator signals to lambda handlers
    // 1. On successful calculation - print the result
    QObject::connect(&calc, &Calculator::resultReady,
                     [&out](double result) {
                         out << "Result: " << result << "\n";
                         out.flush();
                     });
    // 2. On error - print the error message
    QObject::connect(&calc, &Calculator::errorOccurred,
                     [&out](const QString& msg) {
                         out << "Error: " << msg << "\n";
                         out.flush();
                     });
    // Greeting
    out << "=== Qt Console Calculator ===\n";
    printHelp(out);
    out << "\n> ";
    out.flush();
    // Main loop: read lines, parse commands and call calculator methods
    QString line;
    while (in.readLineInto(&line)) {
        line = line.trimmed(); // Remove leading and trailing spaces
        // Empty line - just continue
        if (line.isEmpty()) {
            out << "> ";
            out.flush();
            continue;
        }
        // Split the input into tokens by spaces
        QStringList parts = line.split(' ', Qt::SkipEmptyParts);
        QString command = parts.value(0).toLower();
        // Handle exit commands
        if (command == "quit" || command == "exit") {
            out << "Goodbye!\n";
            break;
        }
        // Help
        if (command == "help") {
            printHelp(out);
            out << "> ";
            out.flush();
            continue;
        }
        // Reset
        if (command == "reset") {
            calc.reset();
            out << "> ";
            out.flush();
            continue;
        }
        // Arithmetic commands require 3 tokens: command + 2 numbers
        if (parts.size() != 3) {
            out << "Error: invalid format. Use: <command> <a> <b>\n";
            out << "> ";
            out.flush();
            continue;
        }
        // Convert operands to numbers
        bool ok1, ok2;
        double a = parts[1].toDouble(&ok1);
        double b = parts[2].toDouble(&ok2);
        if (!ok1 || !ok2) {
            out << "Error: failed to convert operands to numbers\n";
            out << "> ";
            out.flush();
            continue;
        }
        // Execute the requested command
        // A regular method call is used; the signal is emitted inside the method
        if (command == "add") {
            calc.add(a, b);
        }
        else if (command == "sub") {
            calc.subtract(a, b);
        }
        else if (command == "mul") {
            calc.multiply(a, b);
        }
        else if (command == "div") {
            calc.divide(a, b);
        }
        else {
            out << "Unknown command: " << command << "\n";
        }
        out << "> ";
        out.flush();
    }
    // QCoreApplication::exec() is not needed here.
    // We use a blocking input loop instead of the event loop.
    // If QTimer, networking, or other asynchronous functionality
    // were used, app.exec() would be required.
    return 0;
}