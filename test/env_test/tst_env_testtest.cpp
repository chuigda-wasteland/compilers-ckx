#include <QString>
#include <QtTest>

class Env_testTest : public QObject
{
    Q_OBJECT

public:
    Env_testTest();

private Q_SLOTS:
    void testCase1();
};

Env_testTest::Env_testTest()
{
}

void Env_testTest::testCase1()
{
    QVERIFY2(true, "Failure");
}

QTEST_APPLESS_MAIN(Env_testTest)

#include "tst_env_testtest.moc"
