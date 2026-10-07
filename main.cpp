#include <QCoreApplication>
#include <QTextStream>
#include <QStringList>
#include "calculator.h"
#include "logger.h"
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
    QCoreApplication app(argc, argv);
    QTextStream in(stdin);
    QTextStream out(stdout);
    Calculator calc;
    Logger logger;
    QObject::connect(&calc, &Calculator::resultReady,
                     &logger, &Logger::logResult);
    QObject::connect(&calc, &Calculator::errorOccurred,
                     &logger, &Logger::logError);
    QObject::connect(&logger, &Logger::logWritten,
                     [&out](const QString& message) {
                         out << message << "\n";
                         out.flush();
                     });
    QObject::connect(&calc, &Calculator::resultReady,
                     [&out](double result) {
                         out << "Result: " << result << "\n";
                         out.flush();
                     });
    QObject::connect(&calc, &Calculator::errorOccurred,
                     [&out](const QString& message) {
                         out << "Error: " << message << "\n";
                         out.flush();
                     });
    out << "=== Qt Console Calculator ===\n";
    printHelp(out);
    out << "\n> ";
    out.flush();
    QString line;
    while (in.readLineInto(&line)) {
        line = line.trimmed();
        if (line.isEmpty()) {
            out << "> ";
            out.flush();
            continue;
        }
        QStringList parts = line.split(' ', Qt::SkipEmptyParts);
        QString command = parts.value(0).toLower();
        if (command == "quit" || command == "exit") {
            out << "Goodbye!\n";
            break;
        }
        if (command == "help") {
            printHelp(out);
            out << "> ";
            out.flush();
            continue;
        }
        if (command == "reset") {
            calc.reset();
            out << "> ";
            out.flush();
            continue;
        }
        if (parts.size() != 3) {
            out << "Error: invalid format. Use: <command> <a> <b>\n";
            out << "> ";
            out.flush();
            continue;
        }
        bool ok1, ok2;
        double a = parts[1].toDouble(&ok1);
        double b = parts[2].toDouble(&ok2);
        if (!ok1 || !ok2) {
            out << "Error: failed to convert operands to numbers\n";
            out << "> ";
            out.flush();
            continue;
        }
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
    return 0;
}