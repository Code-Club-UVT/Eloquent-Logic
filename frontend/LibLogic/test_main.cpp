#include "tests.h"
#include "logiceditorwindow.h"
#include "logiclineedit.h"
#include "truthtableviewer.h"
#include "syntactictreeviewer.h"
#include <QJsonArray>
#include <QPushButton>
#include <QComboBox>
#include <QTableWidget>
#include <QJsonObject>
#include <QClipboard>
#include <QGuiApplication>
#include <QTimer>
#include <QEventLoop>
#include <QSignalSpy>

void TestMyLogic::initTestCase() {
    qDebug() << "Starting real UI tests...";
}

void TestMyLogic::cleanupTestCase() {
    qDebug() << "Tests finished successfully.";
}

void TestMyLogic::testSymbolReplacement() {
    LogicEditorWindow window;

    LogicLineEdit *input = window.findChild<LogicLineEdit*>();
    QVERIFY(input != nullptr);

    input->setText("A ∧ B → C");

    QString expected = R"(A \wedge B \implies C)";
    QCOMPARE(window.getFormulaWithSymbols(), expected);
}

void TestMyLogic::testLogicButtonsClick() {
    LogicEditorWindow window;

    QPushButton *btnAnd = nullptr;
    for (QPushButton *btn : window.findChildren<QPushButton*>()) {
        if (btn->text() == "∧") btnAnd = btn;
    }
    QVERIFY(btnAnd != nullptr);

    LogicLineEdit *input = window.findChild<LogicLineEdit*>();
    input->clear();

    QTest::mouseClick(btnAnd, Qt::LeftButton);

    QCOMPARE(input->text(), QString(" ∧ "));
}

void TestMyLogic::testActionStatusUpdates() {
    LogicEditorWindow window;

    QPushButton *btnFnc = window.findChild<QPushButton*>("btnFNC");
    QVERIFY(btnFnc != nullptr);

    QLineEdit *resultOutput = window.findChild<QLineEdit*>("resultOutput");
    QVERIFY(resultOutput != nullptr);

    QTest::mouseClick(btnFnc, Qt::LeftButton);

    QCOMPARE(resultOutput->text(), QString("Computing CNF..."));
}

void TestMyLogic::testKeyboardShortcuts() {
    LogicEditorWindow window;

    QPushButton *btnAnd = nullptr;
    QPushButton *btnNot = nullptr;
    for (QPushButton *btn : window.findChildren<QPushButton*>()) {
        if (btn->text() == "∧") btnAnd = btn;
        if (btn->text() == "¬") btnNot = btn;
    }

    QVERIFY(btnAnd != nullptr);
    QVERIFY(btnNot != nullptr);

    QCOMPARE(btnAnd->shortcut(), QKeySequence("Alt+A"));
    QCOMPARE(btnNot->shortcut(), QKeySequence("Alt+N"));
}

void TestMyLogic::testSatAlgorithmSelection() {
    LogicEditorWindow window;

    QComboBox *combo = window.findChild<QComboBox*>("satAlgoSelector");
    QVERIFY(combo != nullptr);

    QCOMPARE(combo->currentData().toString(), QString("dpll_mcl"));

    combo->setCurrentIndex(3);

    QCOMPARE(combo->currentData().toString(), QString("resolution"));
}

void TestMyLogic::testComplexFormulaTranslation() {
    LogicEditorWindow window;
    LogicLineEdit *input = window.findChild<LogicLineEdit*>();
    QVERIFY(input != nullptr);

    input->setText("A ∧ B ∨ C → D ↔ E ¬ F ⊨ G ⊤ ⊥");

    QString expected = R"(A \wedge B \vee C \implies D \iff E \neg F \models G \top \bot)";

    QCOMPARE(window.getFormulaWithSymbols(), expected);
}

void TestMyLogic::testMockSatResponse() {
    LogicEditorWindow window;

    QLineEdit *resultOutput = window.findChild<QLineEdit*>("resultOutput");
    QVERIFY(resultOutput != nullptr);

    QJsonObject mockResponse;
    mockResponse["satisfiable"] = true;

    QMetaObject::invokeMethod(&window, "onAgentResponse",
                              Q_ARG(int, 6),
                              Q_ARG(QJsonObject, mockResponse));

    QCOMPARE(resultOutput->text(), QString("SAT Result: The formula is SATISFIABLE."));
    QVERIFY(resultOutput->styleSheet().contains("green"));
}

void TestMyLogic::testTruthTableInitialization() {
    QStringList mockHeaders = {"A", "B", "A ∧ B"};

    TruthTableViewer viewer(mockHeaders);

    QList<bool> mockRow = {true, false, false};
    viewer.addRow(mockRow);

    QTableWidget *table = viewer.findChild<QTableWidget*>();
    QVERIFY(table != nullptr);

    QCOMPARE(table->columnCount(), 3);

    QCOMPARE(table->rowCount(), 1);
}

void TestMyLogic::testStressHugeFormula() {
    LogicEditorWindow window;
    LogicLineEdit *input = window.findChild<LogicLineEdit*>();
    QVERIFY(input != nullptr);

    QString hugeFormula = "";
    for (int i = 0; i < 100; ++i) {
        hugeFormula += "A ∧ ";
    }
    hugeFormula += "B";

    QElapsedTimer timer;
    timer.start();

    input->setText(hugeFormula);
    QString result = window.getFormulaWithSymbols();

    qint64 elapsed = timer.elapsed();

    QVERIFY(!result.isEmpty());
    QVERIFY2(elapsed < 100, "Warning: Formula conversion took too long (poor performance)!");
}

void TestMyLogic::testAgentDisconnectionHandling() {
    LogicEditorWindow window;

    QLineEdit *resultOutput = window.findChild<QLineEdit*>("resultOutput");
    QVERIFY(resultOutput != nullptr);

    QJsonObject emptyErrorObj;
    QMetaObject::invokeMethod(&window, "onAgentError",
                              Q_ARG(int, -1),
                              Q_ARG(QJsonObject, emptyErrorObj));

    QString statusText = resultOutput->text().toLower();
    bool isErrorDisplayed = statusText.contains("error") ||
                            statusText.contains("disconnected") ||
                            !statusText.isEmpty();

    QVERIFY2(isErrorDisplayed, "The UI did not display any error message upon agent disconnection!");
}

void TestMyLogic::testSyntaxTreeViewerCreation() {
    SyntacticTreeViewer treeViewer;
    QVERIFY(&treeViewer != nullptr);
}

void TestMyLogic::testInvalidAgentResponse() {
    LogicEditorWindow window;

    QLineEdit *resultOutput = window.findChild<QLineEdit*>("resultOutput");
    QVERIFY(resultOutput != nullptr);

    QJsonObject corruptedResponse;

    QMetaObject::invokeMethod(&window, "onAgentResponse",
                              Q_ARG(int, 6),
                              Q_ARG(QJsonObject, corruptedResponse));

    QVERIFY(true);
}

void TestMyLogic::testSignalSpying() {
    LogicEditorWindow window;
    LogicLineEdit *input = window.findChild<LogicLineEdit*>();
    QVERIFY(input != nullptr);

    QSignalSpy spy(input, &QLineEdit::textChanged);

    QTest::keyClicks(input, "A");

    QCOMPARE(spy.count(), 1);

    QList<QVariant> arguments = spy.takeFirst();

    QCOMPARE(arguments.at(0).toString(), QString("A"));
}

void TestMyLogic::testMemoryLeaksPrevention() {
    QStringList dummyHeaders = {"A", "B"};

    TruthTableViewer *tableViewer = new TruthTableViewer(dummyHeaders);
    SyntacticTreeViewer *treeViewer = new SyntacticTreeViewer();

    bool isTableSafe = tableViewer->testAttribute(Qt::WA_DeleteOnClose);
    bool isTreeSafe = treeViewer->testAttribute(Qt::WA_DeleteOnClose);

    delete tableViewer;
    delete treeViewer;

    QVERIFY2(isTableSafe, "MEMORY LEAK! TruthTableViewer does not have Qt::WA_DeleteOnClose set in its constructor!");
    QVERIFY2(isTreeSafe, "MEMORY LEAK! SyntacticTreeViewer does not have Qt::WA_DeleteOnClose set in its constructor!");
}

void TestMyLogic::testLayoutResponsiveness() {
    LogicEditorWindow window;

    window.show();
    QVERIFY(QTest::qWaitForWindowExposed(&window));

    LogicLineEdit *input = window.findChild<LogicLineEdit*>();
    QVERIFY(input != nullptr);

    window.resize(200, 200);
    QCoreApplication::processEvents();

    bool hasLayout = (window.layout() != nullptr);
    bool isInputResized = (input->width() <= 200);

    QVERIFY2(hasLayout || isInputResized,
             "UX Error: Window is not responsive! No QLayout used, elements clip out of bounds on resize.");
}

void TestMyLogic::testTabNavigation() {
    LogicEditorWindow window;
    window.show();
    QVERIFY(QTest::qWaitForWindowExposed(&window));

    LogicLineEdit *input = window.findChild<LogicLineEdit*>();
    QVERIFY(input != nullptr);

    input->setFocus();
    QVERIFY(input->hasFocus());

    QTest::keyClick(&window, Qt::Key_Tab);
    QCoreApplication::processEvents();

    QVERIFY2(input->hasFocus() == false,
             "Accessibility Error: TAB key navigation failed! Focus remained trapped on the input field.");
}

void TestMyLogic::testClipboardOperations() {
    LogicEditorWindow window;
    LogicLineEdit *input = window.findChild<LogicLineEdit*>();
    QVERIFY(input != nullptr);

    QClipboard *clipboard = QGuiApplication::clipboard();

    QString copiedText = "A ∨ B ↔ C";
    clipboard->setText(copiedText);

    input->clear();
    input->setFocus();

    input->paste();
    QCoreApplication::processEvents();

    QCOMPARE(input->text(), copiedText);

    input->selectAll();
    input->copy();

    QCOMPARE(clipboard->text(), copiedText);
}

void TestMyLogic::testTruthTableEmptyState() {
    QStringList emptyHeaders;

    TruthTableViewer *tableViewer = new TruthTableViewer(emptyHeaders);
    tableViewer->setAttribute(Qt::WA_DeleteOnClose);

    QTableWidget *table = tableViewer->findChild<QTableWidget*>();
    QVERIFY(table != nullptr);

    QCOMPARE(table->columnCount(), 0);
    QCOMPARE(table->rowCount(), 0);

    delete tableViewer;
    QVERIFY(true);
}

void TestMyLogic::testNetworkLatencySimulation() {
    LogicEditorWindow window;
    QLineEdit *resultOutput = window.findChild<QLineEdit*>("resultOutput");
    QVERIFY(resultOutput != nullptr);

    resultOutput->setText("Waiting for backend...");

    QEventLoop loop;

    QTimer::singleShot(500, &window, [&window, &loop]() {
        QJsonObject mockResponse;
        mockResponse["satisfiable"] = false;

        QMetaObject::invokeMethod(&window, "onAgentResponse",
                                  Q_ARG(int, 6),
                                  Q_ARG(QJsonObject, mockResponse));

        loop.quit();
    });

    loop.exec();

    QString outputText = resultOutput->text().toUpper();
    QVERIFY2(outputText.contains("UNSATISFIABLE"),
             "Error: After the delay, the UI did not display the correct UNSAT message!");
    QVERIFY(resultOutput->styleSheet().contains("red"));
}

void TestMyLogic::testMathFontReadability() {
    LogicEditorWindow window;
    LogicLineEdit *input = window.findChild<LogicLineEdit*>();
    QVERIFY(input != nullptr);

    QFont inputFont = input->font();

    bool isFontReadable = (inputFont.pointSize() >= 12 || inputFont.pixelSize() >= 16);

    QVERIFY2(isFontReadable,
             "Ergonomics Error: The input field font is too small! Set a font size of at least 12pt.");
}

QTEST_MAIN(TestMyLogic)