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
        else return saber::char_traits<qchar>::eof();
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
#define GGLEX(X, Y) ckx_token::type::token_##Y,
#include "gg.h"
#undef GGLEX
    };

    ckx_test_filereader reader = ckx_test_filereader(saber::move(str));
    ckx_default_token_stream stream = ckx_default_token_stream(reader);

    for (qsizet i = 0;
         stream[0].get()->token_type != ckx_token::type::token_eoi;
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

    saber::vector<saber_ptr<ckx_token>> comparsion =
    {
#define VYLEX(X) saber_ptr<ckx_token>(new ckx_token(qcoord(0, 0), saber::string(X))),
#include "vy.h"
#undef VYLEX
        saber_ptr<ckx_token>(new ckx_token(qcoord(0, 0), ""))
    };

    for (qsizet i = 0;
         stream[0].get()->token_type != ckx_token::type::token_eoi;
         ++i, ++stream)
    {
        if (stream[0].get()->token_type != ckx_token::type::token_identifier
            || *(stream[0].get()->v.p_str) != *(comparsion[i].get()->v.p_str))
        {
            qDebug() << "Failed at token" << i << '\n';
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
    void test_tokenstream_1();
    void test_tokenstream_2();
};

DefaultTokenStreamTest::DefaultTokenStreamTest()
{
}

void DefaultTokenStreamTest::test_tokenstream_1()
{
    QVERIFY2(ckx::do_test_ckx_token_stream_1(), "Failure");
}

void DefaultTokenStreamTest::test_tokenstream_2()
{
    QVERIFY2(ckx::do_test_ckx_token_stream_2(), "Failure");
}

QTEST_APPLESS_MAIN(DefaultTokenStreamTest)

#include "tst_default_tokenstreamtest.moc"
