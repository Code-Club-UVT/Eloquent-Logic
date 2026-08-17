#ifndef TEST_MYLOGIC_H
#define TEST_MYLOGIC_H

#include <QObject>
#include <QtTest>

class TestMyLogic : public QObject {
    Q_OBJECT

private slots:
    void initTestCase();
    void cleanupTestCase();
    void testSymbolReplacement();
    void testLogicButtonsClick();
    void testActionStatusUpdates();
    void testKeyboardShortcuts();
    void testSatAlgorithmSelection();
    void testComplexFormulaTranslation();
    void testMockSatResponse();
    void testTruthTableInitialization();
    void testStressHugeFormula();
    void testAgentDisconnectionHandling();
    void testSyntaxTreeViewerCreation();
    void testInvalidAgentResponse();
    void testSignalSpying();
    void testMemoryLeaksPrevention();
    void testLayoutResponsiveness();
    void testTabNavigation();
    void testClipboardOperations();
    void testTruthTableEmptyState();
    void testNetworkLatencySimulation();
    void testMathFontReadability();
};

#endif