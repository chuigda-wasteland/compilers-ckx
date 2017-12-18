/**
    The Opensource Compiler CKX -- for my honey ChenKX
    Copyright (C) 2017  CousinZe

    This program is free software: you can redistribute it and/or modify
    it under the terms of the GNU Affero General Public License as published
    by the Free Software Foundation, either version 3 of the License, or
    (at your option) any later version.

    This program is distributed in the hope that it will be useful,
    but WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
    GNU Affero General Public License for more details.

    You should have received a copy of the GNU Affero General Public License
    along with this program.  If not, see http://www.gnu.org/licenses/.
  */

#include "ckx_type.hpp"

#include <cstdio>
#include <cstdlib>

using namespace std;
using namespace ckx;
using namespace saber;

void assert_printlog(bool _condition, const char* _desc)
{
    if (!_condition)
    {
        printf("%s\n", _desc);
        abort();
    }
}

void testcase1()
{
    saber_ptr<ckx_type> sktt1 = ckx_type_helper::get_vi8_type();
    saber_ptr<ckx_type> sktt2 = ckx_type_helper::get_vi8_type();

    auto relation = ckx_type_helper::resolve_relation(sktt1, sktt2);
    assert_printlog(relation == ckx_type_helper::relation::rel_equal,
                    "vi8 not equal to vi8.");
}

void testcase2()
{
    saber_ptr<ckx_type> sktt1 = ckx_type_helper::get_vi8_type();
    saber_ptr<ckx_type> sktt2 = ckx_type_helper::get_vi16_type();

    auto relation = ckx_type_helper::resolve_relation(sktt1, sktt2);
    assert_printlog(relation == ckx_type_helper::relation::rel_comptiable,
                    "vi8 not comptiable with vi16");
}

void testcase3()
{
    saber_ptr<ckx_type> sktt1 = ckx_type_helper::get_vi8_type();
    saber_ptr<ckx_type> sktt2 = ckx_type_helper::get_vu8_type();
    auto relation = ckx_type_helper::resolve_relation(sktt1, sktt2);
    assert_printlog(relation == ckx_type_helper::relation::rel_can_cast,
                    "cannot make static cast from vi8 to vu8");
}

void testcase4()
{
    saber_ptr<ckx_type> sktt1 = ckx_type_helper::get_vi8_type();
    saber_ptr<ckx_type> sktt2 =
        ckx_type_helper::qual_const(ckx_type_helper::get_vu8_type());

    auto relation = ckx_type_helper::resolve_relation(sktt1, sktt2);
    assert_printlog(relation == ckx_type_helper::relation::rel_can_cast,
                    "casting between vi8 and const vu8 requires reinterpret");
}

void testcase5()
{
    saber_ptr<ckx_type> vu8_type = ckx_type_helper::get_vu8_type();
    saber::vector<saber_ptr<ckx_type>> vecparam
    {
        ckx_type_helper::get_vu8_type(),
        ckx_type_helper::get_vu16_type(),
        ckx_type_helper::get_vr64_type(),
    };

    saber::vector<saber_ptr<ckx_type>> vecparam1 = vecparam;

    saber_ptr<ckx_type> func1 =
        new ckx_func_type(vu8_type, saber::move(vecparam));
    saber_ptr<ckx_type> func2 =
        new ckx_func_type(vu8_type, saber::move(vecparam1));

    auto relation = ckx_type_helper::resolve_func_relation(func1, func2);
    assert_printlog(relation == ckx_type_helper::func_relation::rel_same,
                    "(vu8,vu16,vr64)->vu8 not equal to itself");
}

void testcase6()
{
    saber_ptr<ckx_type> vu8_type = ckx_type_helper::get_vu8_type();
    saber_ptr<ckx_type> vi8_type = ckx_type_helper::get_vi8_type();

    saber::vector<saber_ptr<ckx_type>> vecparam
    {
        ckx_type_helper::get_vu8_type(),
        ckx_type_helper::get_vu16_type(),
        ckx_type_helper::get_vr64_type(),
    };

    saber::vector<saber_ptr<ckx_type>> vecparam1 = vecparam;

    saber_ptr<ckx_type> func1 =
        new ckx_func_type(vu8_type, saber::move(vecparam));
    saber_ptr<ckx_type> func2 =
        new ckx_func_type(vu8_type, saber::move(vecparam1));

    auto relation = ckx_type_helper::resolve_func_relation(func1, func2);
    assert_printlog(relation==ckx_type_helper::func_relation::rel_incomptiable,
                    "(vu8,vu16,vr64)->vu8 comptiable with "
                    "(vu8,vu16,vr64)->vi8");
}

void testcase7()
{
    saber_ptr<ckx_type> vu8_type = ckx_type_helper::get_vu8_type();
    saber::vector<saber_ptr<ckx_type>> vecparam
    {
        ckx_type_helper::get_vu8_type(),
        ckx_type_helper::get_vu16_type(),
        ckx_type_helper::get_vr64_type(),
    };

    saber::vector<saber_ptr<ckx_type>> vecparam1
    {
        ckx_type_helper::get_vu16_type(),
        ckx_type_helper::get_vu32_type(),
        ckx_type_helper::get_vr32_type(),
    };

    saber_ptr<ckx_type> func1 =
        new ckx_func_type(vu8_type, saber::move(vecparam));
    saber_ptr<ckx_type> func2 =
        new ckx_func_type(vu8_type, saber::move(vecparam1));

    auto relation = ckx_type_helper::resolve_func_relation(func1, func2);
    assert_printlog(relation==ckx_type_helper::func_relation::rel_incomptiable,
                    "(vu8,vu16,vr64)->vu8 comptiable with "
                    "(vu16,vu32,vr32)->vu8");
}

void testcase8()
{
    saber_ptr<ckx_type> vu8_type = ckx_type_helper::get_vu8_type();
    saber::vector<saber_ptr<ckx_type>> vecparam
    {
        ckx_type_helper::get_vu8_type(),
        ckx_type_helper::get_vu16_type(),
        ckx_type_helper::get_vr64_type(),
    };

    saber::vector<saber_ptr<ckx_type>> vecparam1
    {
        ckx_type_helper::get_vch_type(),
        ckx_type_helper::get_vu16_type(),
        ckx_type_helper::get_vr64_type(),
    };

    saber_ptr<ckx_type> func1 =
        new ckx_func_type(vu8_type, saber::move(vecparam));
    saber_ptr<ckx_type> func2 =
        new ckx_func_type(vu8_type, saber::move(vecparam1));

    auto relation = ckx_type_helper::resolve_func_relation(func1, func2);
    assert_printlog(relation == ckx_type_helper::func_relation::rel_overload,
                    "(vu8,vu16,vr64)->vu8 cannot overload with "
                    "(vch,vu16,vr64)->vu8");
}

int main()
{
    testcase1();
    testcase2();
    testcase3();
    testcase4();
    testcase5();
    testcase6();
    testcase7();
    testcase8();
    printf("Test for type system finished successfully.\n");
    return 0;
}
