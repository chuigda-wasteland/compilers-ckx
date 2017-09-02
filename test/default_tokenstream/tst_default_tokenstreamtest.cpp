#include <QString>
#include <QtTest>

#include "ckx_file_reader.hpp"
#include "ckx_token_stream.hpp"
#include "defs.hpp"

namespace ckx
{

class ckx_test_filereader implements ckx_file_reader
{
public:
    ckx_test_filereader(saber::string&& _str) : str(saber::move(_str)) {}
    ~ckx_test_filereader() = default;
    qchar get_next_char(void) override final
    {
        pos++;
        if (pos-1 < str.size()) return str[pos-1];
        else return static_cast<qchar>(saber::char_traits<qchar>::eof());
    }

private:
    qsizet pos = 0;
    saber::string str;
};

static bool do_test_ckx_token_stream_1(void)
{
    saber::string str =
#define GGLEX(X, Y) X" "
#include "gg.h"
#undef GGLEX
            ;

    saber::vector<ckx_token::type> tokens =
    {
#define GGLEX(X, Y) ckx_token::type::tk_##Y,
#include "gg.h"
#undef GGLEX
    };

    ckx_test_filereader reader = ckx_test_filereader(saber::move(str));
    ckx_default_token_stream stream = ckx_default_token_stream(reader);

    for (qsizet i = 0;
         stream[0].get()->token_type != ckx_token::type::tk_eoi;
         ++i, ++stream)
    {
        if (stream[0].get()->token_type != tokens[i])
        {
            qDebug() << "Failed at line" << i << '\n';
            return false;
        }
    }

    return true;
}

static bool do_test_ckx_token_stream_2(void)
{
    saber::string str =
#define VYLEX(X) X" "
#include "vy.h"
#undef VYLEX
    ;

    ckx_test_filereader reader = ckx_test_filereader(saber::move(str));
    ckx_default_token_stream stream = ckx_default_token_stream(reader);

    saber::vector<saber::string> comparsion =
    {
#define VYLEX(X) saber::string(X),
#include "vy.h"
#undef VYLEX
        ""
    };

    for (qsizet i = 0;
         stream[0].get()->token_type != ckx_token::type::tk_eoi;
         ++i, ++stream)
    {
        if (stream[0].get()->token_type != ckx_token::type::tk_id
            || *(stream[0].get()->v.p_str) != comparsion[i])
        {
            qDebug() << "Failed at token" << i << '\n';
            return false;
        }
    }

    return true;
}

static bool float_equal(qreal r1, qreal r2)
{
    return (r1 - r2) < std::numeric_limits<qreal>::epsilon();
}

static bool do_test_ckx_token_stream_3()
{
    saber::string str =
#define VY2LEX(X, Y) X" "
#include "vy2.h"
#undef VY2LEX
            ;

    qreal comparsion[] =
    {
#define VY2LEX(X, Y) Y,
#include "vy2.h"
#undef VY2LEX
    };

    ckx_test_filereader reader = ckx_test_filereader(saber::move(str));
    ckx_default_token_stream stream = ckx_default_token_stream(reader);

    for (qsizet i = 0;
         stream[0].get()->token_type != ckx_token::type::tk_eoi;
         ++i, ++stream)
    {
        if (stream[0].get()->token_type != ckx_token::type::tk_vr_literal
            || !float_equal(stream[0].get()->v.r, comparsion[i]) )
        {
            qDebug() << "Failed at token" << i << '\n';
            qDebug() << stream[0].get()->v.r << '\n';
            qDebug() << comparsion[i] << '\n';
            return false;
        }
    }

    return true;
}

static bool do_test_ckx_token_stream_4()
{
    saber::string str =
#define VY3LEX(X, Y) X" "
#include "vy3.h"
#undef VY3LEX
            ;

    qint64 comparsion[] =
    {
#define VY3LEX(X, Y) Y,
#include "vy3.h"
#undef VY3LEX
    };

    ckx_test_filereader reader = ckx_test_filereader(saber::move(str));
    ckx_default_token_stream stream = ckx_default_token_stream(reader);

    for (qsizet i = 0;
         stream[0].get()->token_type != ckx_token::type::tk_eoi;
         ++i, ++stream)
    {
        if (stream[0].get()->token_type != ckx_token::type::tk_vi_literal
            || stream[0].get()->v.i64 != comparsion[i] )
        {
            qDebug() << "Failed at token" << i << '\n';
            qDebug() << stream[0].get()->v.i64 << '\n';
            qDebug() << comparsion[i] << '\n';
            return false;
        }
    }

    return true;
}

static bool do_test_ckx_token_stream_5()
{
    saber::string str =
#define VY4LEX(X, Y) X" "
#include "vy4.h"
#undef VY4LEX
            ;

    ckx_token::type comparsion[] =
    {
#define VY4LEX(X, Y) ckx_token::type::tk_##Y,
#include "vy4.h"
#undef VY4LEX
    };

    ckx_test_filereader reader = ckx_test_filereader(saber::move(str));
    ckx_default_token_stream stream = ckx_default_token_stream(reader);

    for (qsizet i = 0;
         stream[0].get()->token_type != ckx_token::type::tk_eoi;
         ++i, ++stream)
    {
        if (stream[0].get()->token_type != comparsion[i])
        {
            qDebug() << "Failed at token" << i << '\n';
            qDebug() << (int)stream[0].get()->token_type << '\n';
            qDebug() << (int)comparsion[i] << '\n';
            return false;
        }
    }

    return true;
}

} // namespace ckx


class DefaultTokenStreamTest : public QObject
{
    Q_OBJECT

public:
    DefaultTokenStreamTest();

private Q_SLOTS:
    void test_tokenstream_1(); // testcase for keywords
    void test_tokenstream_2(); // testcase for identifiers
    void test_tokenstream_3(); // testcase for real literals
    void test_tokenstream_4(); // testcase for integers
    void test_tokenstream_5(); // testcase for symbols
};

DefaultTokenStreamTest::DefaultTokenStreamTest()
{
}

void DefaultTokenStreamTest::test_tokenstream_1()
{
    QVERIFY2(ckx::do_test_ckx_token_stream_1(), "Failure in test for kwds");
}

void DefaultTokenStreamTest::test_tokenstream_2()
{
    QVERIFY2(ckx::do_test_ckx_token_stream_2(), "Failure in test for ids");
}

void DefaultTokenStreamTest::test_tokenstream_3()
{
    QVERIFY2(ckx::do_test_ckx_token_stream_3(), "Failure in test for reals");
}

void DefaultTokenStreamTest::test_tokenstream_4()
{
    QVERIFY2(ckx::do_test_ckx_token_stream_4(), "Failure in test for integers");
}

void DefaultTokenStreamTest::test_tokenstream_5()
{
    QVERIFY2(ckx::do_test_ckx_token_stream_5(), "Failure in test for symbols");
}

QTEST_APPLESS_MAIN(DefaultTokenStreamTest)

#include "tst_default_tokenstreamtest.moc"
