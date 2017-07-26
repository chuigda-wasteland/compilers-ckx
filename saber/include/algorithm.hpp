// ///////////////////////////////////////////////////////////////////////// //

//  たとえば途切れた空が               -  你若看见了                         //
//  見えたなら                         -  那断裂的苍空                       //
//  震える僕の声が聞こえるのなら       -  你若听见了我颤抖的歌声             //
//  バラバラに砕けるほど               -  纵使被击成粉碎                     //
//  舞い上がれ                         -  亦要向苍空飞舞                     //
//  引き裂かれた                       -  支离破碎的记忆                     //
//  記憶の果敢なきツバサ               -  无尽飘飞的羽翼                     //
//  あの日語り合ったこと               -  那一天的促膝长谈                   //
//  いつも笑い合えたこと               -  昔日里的谈笑风生                   //
//  よみがえる日まで                   -  在这一切复苏之前                   //
//  立ち上がるだけ                     -  唯有再度振奋                       //

//  壊してもっともっと僕を感じて       -  破坏吧 更多地 更多地感受我         //
//  そこにそこに君はいますか           -  那里 那里 是否你就在那里           //
//  戦場に咲く命よ燃えろ燃えろ         -  战场上绽放的生命 燃烧吧 燃烧吧     //
//  殺していっそいっそ                 -  索性 索性就此扼杀一切              //
//  朽ち果てるなら                     -  如果万物终将走向湮灭               //
//  たぎれたぎれ破滅の果てに           -  重拾心中的沸腾 在那破灭的尽头      //
//  奇跡を呼び覚ませ                   -  唤醒奇迹                           //
//  閉ざされた空へ                     -  飞向那片封锁的苍空                 //

//  飛び交う無数の感覚のなかで         -  纷飞交错的无数感受之中             //
//  本当の自分さえも失くしてしまう     -  连真正的自我也迷失不再             //
//  見えない不安を集中砲火に           -  集中火力对抗隐形的不安             //
//  勝ち残るのは弱さ認める強さ         -  敢于承认自身的弱小 方能赢到最后    //
//  もしも僕じゃなかったら             -  如若我不是我                       //
//  もしも君じゃなかったら             -  你不是你                           //
//  こんな気持ちさえ                   -  就连这份心情                       //
//  知らずにいたね                     -  我也不曾知晓                       //
//  切り裂けもっともっと               -  撕裂苍空 竭力飞向                  //
//  正義の闇へ                         -  那正义的黑暗                       //

//  はしれはしれ灰になるまで           -  奔跑吧 奔跑吧 直至燃为灰烬         //
//  理屈を捨てて心で                   -  舍弃理性                           //
//  吠えろ吠えろ                       -  发自真心地嘶吼吧 咆哮吧            //
//  断ち切れやがてやがて               -  斩断阻隔                           //
//  生まれる銀河に                     -  在那终获重生的银河之处             //
//  君が君が居てくれるなら             -  只要你 你还在我的身边              //
//  僕らのかがやきは                   -  我们的这份光芒                     //
//  無敵にもなれる                     -  定将无人能敌                       //

//  右にならえと                       -  只要随波逐流                       //
//  誰もが今日を生きてる               -  就能幸免于难                       //
//  もどかしさに理由もないまま         -  心浮气躁 缘由依然无从得知          //
//  死んだみたいに生きてくよりも       -  与其行尸走肉般苟且偷生             //
//  赤い血を流し牙を剥け               -  不如挥洒赤血大放异彩               //
//  それが僕が君が生きてる証明         -  那是我 也是你的生存证明            //

//  壊してもっともっと僕を感じて       -  破坏吧 更多地 更多地感受我         //
//  そこにそこに君はいますか           -  那里 那里 是否你就在那里           //
//  戦場に咲く命よ燃えろ燃えろ         -  战场上绽放的生命 燃烧吧 燃烧吧     //
//  殺していっそいっそ                 -  索性 索性就此扼杀一切              //
//  朽ち果てるなら                     -  如果万物终将走向湮灭               //
//  たぎれたぎれ破滅の果てに           -  重拾心中的沸腾 在那破灭的尽头      //
//  奇跡を呼び覚ませ                   -  唤醒奇迹                           //
//  閉ざされた空へ                     -  飞向那片封锁的苍空                 //
//
// ///////////////////////////////////////////////////////////////////////// //

#ifdef _MSC_VER
#pragma once
#endif

#ifndef ALGORITHM_HPP
#define ALGORITHM_HPP

#include "memory.hpp"
#include "iterator.hpp"
#include "saber_traits.hpp"

namespace saber
{

template <typename InputIterator, typename OutputIterator>
OutputIterator
copy(InputIterator _first, InputIterator _last, OutputIterator _d_first)
{
    static_assert(traits::is_input_iterator<InputIterator>::value,
                  C8_STAT__TEMPLATE_ARG__ITER__NOT_INPUT_ITERATOR);
    static_assert(traits::is_output_iterator<OutputIterator>::value,
                  C8_STAT__TEMPLATE_ARG__ITER__NOT_OUTPUT_ITERATOR);

    while (_first != _last)
    {
        *_d_first++ = *_first++;
    }

    return _d_first;
}

template <typename InputIterator, typename OutputIterator, typename Predicator>
OutputIterator
copy_if(InputIterator _first,
        InputIterator _last,
        OutputIterator _d_first,
        Predicator _pred)
{
    static_assert(traits::is_input_iterator<InputIterator>::value,
                  C8_STAT__TEMPLATE_ARG__ITER__NOT_INPUT_ITERATOR);
    static_assert(traits::is_output_iterator<OutputIterator>::value,
                  C8_STAT__TEMPLATE_ARG__ITER__NOT_OUTPUT_ITERATOR);

    while (_first != _last)
    {
        if (_pred(*_first))
        {
            *_d_first++ = *_first;
        }
        _first++;
    }
    return _d_first;
}

template <typename InputIterator, typename Size, typename OutputIterator>
OutputIterator
copy_n(InputIterator _first, Size _count, OutputIterator _d_first)
{
    if (_count > 0)
    {
        *_d_first++ = *_first;
        for (Size i = 1; i < _count; ++i)
        {
            *_d_first++ = *++_first;
        }
    }
    return _d_first;
}

template <typename BidirectionalIterator, typename  OutputIterator>
OutputIterator
reverse_copy(BidirectionalIterator _first,
             BidirectionalIterator _last,
             OutputIterator _d_first)
{
    while (_first != _last)
    {
        --_last;
        *_d_first = *_last;
        ++_d_first;
    }
    return _d_first;
}

template <typename ForwardIterator, typename T>
void
fill(ForwardIterator _first, ForwardIterator _last, const T& _value)
{
    for (; _first != _last; ++_first)
    {
        *_first = _value;
    }
}

template <typename OutputIterator, typename Size, typename T>
OutputIterator
fill_n(OutputIterator _first, Size _count, const T& _value)
{
    for (Size i = 0; i < _count; i++)
    {
        *(_first++) = _value;
    }
    return _first;
}

template <typename RandomAccessIterator>
typename iterator_traits<RandomAccessIterator>::difference_type
distance_impl(RandomAccessIterator _first, RandomAccessIterator _last,
              traits::true_type)
{
    return _last - _first;
}

template <typename InputIterator>
typename iterator_traits<InputIterator>::difference_type
distance_impl(InputIterator _first, InputIterator _last,
              traits::false_type)
{
    typename iterator_traits<InputIterator>::difference_type size = 0;
    for (; _first != _last; ++_first, ++size);
    return size;
}

template <typename InputIterator>
typename iterator_traits<InputIterator>::difference_type
distance(InputIterator _first, InputIterator _last)
{
    return
        distance_impl(_first, _last,
                      traits::is_random_access_iterator<InputIterator>::value);
}

using std::swap;

template <typename RandomAccessIterator>
RandomAccessIterator
_internal_partition(RandomAccessIterator _first, RandomAccessIterator _last)
{
    RandomAccessIterator guard = _first;

    while (_first != _last)
    {
        while (!(*_last < *guard) && _first != _last) _last--;
        while (!(*guard < *_first) && _first != _last) _first++;

        if (_first != _last) swap(*_first, *_last);
    }
    swap(*_first, *guard);
    return _first;
}

template <typename RandomAccessIterator>
void
sort(RandomAccessIterator _first, RandomAccessIterator _last)
{
    static_assert(
        saber::traits::is_random_access_iterator<RandomAccessIterator>::value,
        C8_STAT__TEMPLATE_ARG__ITER__NOT_RANDOM_ACCESS_ITERATOR);

    if (!(_first < _last)) return;

    RandomAccessIterator mid = _internal_partition(_first, _last);
    sort(_first, mid-1);
    sort(mid+1, _last);
}

} // namespace saber

#endif // ALGORITHM_HPP
