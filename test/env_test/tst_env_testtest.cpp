#include <QString>
#include <QtTest>

#include "ckx_env_table.hpp"

class Env_testTest : public QObject
{
    Q_OBJECT

public:
    Env_testTest();

private Q_SLOTS:
    void testCaseAddVar();
    void testCaseAddType();
    void testCaseAddFunc(); /// @todo : finish this part!
};

Env_testTest::Env_testTest()
{
}

void Env_testTest::testCaseAddVar()
{
    using namespace ckx;
    ckx_env* env = new ckx_env(nullptr);

    saber_string_view var_name = saber_string_pool::create_view("a");
    saber_ptr<ckx_type> var_type =
            ckx_type_helper::get_type(ckx_token::type::tk_vi8);
    ckx_var_entry* entry = env->add_new_var(var_name, var_type).second;
    QVERIFY(entry->var_name == var_name);
    QVERIFY(entry->var_type->get_category() == var_type->get_category());
    delete env;
}

void Env_testTest::testCaseAddType()
{
    using namespace ckx;
    ckx_env* env = new ckx_env(nullptr);

    saber_string_view type_name = saber_string_pool::create_view("Lex");
    saber_ptr<ckx_type> type =
            ckx_type_helper::pointer_to(
                ckx_type_helper::qual_const(
                    ckx_type_helper::get_type(ckx_token::type::tk_vi8)));
    ckx_type_entry* entry = env->add_new_type(type_name, type).second;
    QVERIFY(entry->type_name == type_name);
    qDebug() << type->to_string().c_str();
    delete env;
}

void Env_testTest::testCaseAddFunc()
{
    using namespace ckx;
    QVERIFY(true);
}

QTEST_APPLESS_MAIN(Env_testTest)

#include "tst_env_testtest.moc"
