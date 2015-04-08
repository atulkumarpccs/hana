/*
@copyright Louis Dionne 2015
Distributed under the Boost Software License, Version 1.0.
(See accompanying file LICENSE.md or copy at http://boost.org/LICENSE_1_0.txt)
 */

#include <boost/hana/detail/variadic/foldr1.hpp>

#include <boost/hana/assert.hpp>

#include <laws/base.hpp>
using namespace boost::hana;


struct undefined { };

template <int i>
using x = test::ct_eq<i>;

// We do not use test::_injection here because comparing the result would
// blow away the template recursion limit.
struct _f {
    template <typename X, typename Y>
    constexpr auto operator()(X const&, Y const&) {
        return x<value<X>() - value<Y>()>{};
    }
};

int main() {
    using detail::variadic::foldr1;
    _f f{};

    BOOST_HANA_CONSTANT_CHECK(equal(
        foldr1(undefined{}, x<0>{}),
        x<0>{}
    ));

    BOOST_HANA_CONSTANT_CHECK(equal(
        foldr1(f, x<0>{}, x<1>{}),
        f(x<0>{}, x<1>{})
    ));

    BOOST_HANA_CONSTANT_CHECK(equal(
        foldr1(f, x<0>{}, x<1>{}, x<2>{}),
        f(x<0>{}, f(x<1>{}, x<2>{}))
    ));

    BOOST_HANA_CONSTANT_CHECK(equal(
        foldr1(f, x<0>{}, x<1>{}, x<2>{}, x<3>{}),
        f(x<0>{}, f(x<1>{}, f(x<2>{}, x<3>{})))
    ));

    BOOST_HANA_CONSTANT_CHECK(equal(
        foldr1(f, x<0>{}, x<1>{}, x<2>{}, x<3>{}, x<4>{}),
        f(x<0>{}, f(x<1>{}, f(x<2>{}, f(x<3>{}, x<4>{}))))
    ));

    BOOST_HANA_CONSTANT_CHECK(equal(
        foldr1(f, x<0>{}, x<1>{}, x<2>{}, x<3>{}, x<4>{}, x<5>{}),
        f(x<0>{}, f(x<1>{}, f(x<2>{}, f(x<3>{}, f(x<4>{}, x<5>{})))))
    ));

    BOOST_HANA_CONSTANT_CHECK(equal(
        foldr1(f, x<0>{}, x<1>{}, x<2>{}, x<3>{}, x<4>{}, x<5>{}, x<6>{}),
        f(x<0>{}, f(x<1>{}, f(x<2>{}, f(x<3>{}, f(x<4>{}, f(x<5>{}, x<6>{}))))))
    ));

    BOOST_HANA_CONSTANT_CHECK(equal(
        foldr1(f, x<0>{}, x<1>{}, x<2>{}, x<3>{}, x<4>{}, x<5>{}, x<6>{}, x<7>{}),
        f(x<0>{}, f(x<1>{}, f(x<2>{}, f(x<3>{}, f(x<4>{}, f(x<5>{}, f(x<6>{}, x<7>{}
        )))))))
    ));

    BOOST_HANA_CONSTANT_CHECK(equal(
        foldr1(f, x<0>{}, x<1>{}, x<2>{}, x<3>{}, x<4>{}, x<5>{}, x<6>{}, x<7>{}, x<8>{}),
        f(x<0>{}, f(x<1>{}, f(x<2>{}, f(x<3>{}, f(x<4>{}, f(x<5>{}, f(x<6>{}, f(x<7>{}, x<8>{}
        ))))))))
    ));


    BOOST_HANA_CONSTANT_CHECK(equal(
        foldr1(f, x<1>{}, x<2>{}, x<3>{},  x<4>{},  x<5>{},  x<6>{}, x<7>{},
                  x<8>{}, x<9>{}, x<10>{}, x<11>{}, x<12>{}, x<13>{}),
        f(x<1>{}, f(x<2>{}, f(x<3>{}, f(x<4>{}, f(x<5>{}, f(x<6>{}, f(x<7>{},
        f(x<8>{}, f(x<9>{}, f(x<10>{}, f(x<11>{}, f(x<12>{}, x<13>{}
        ))))))))))))
    ));

    BOOST_HANA_CONSTANT_CHECK(equal(
        foldr1(f, x<1>{}, x<2>{}, x<3>{},  x<4>{},  x<5>{},  x<6>{}, x<7>{},
                  x<8>{}, x<9>{}, x<10>{}, x<11>{}, x<12>{}, x<13>{}, x<14>{}),
        f(x<1>{}, f(x<2>{}, f(x<3>{}, f(x<4>{}, f(x<5>{}, f(x<6>{}, f(x<7>{},
        f(x<8>{}, f(x<9>{}, f(x<10>{}, f(x<11>{}, f(x<12>{}, f(x<13>{}, x<14>{}
        )))))))))))))
    ));

    BOOST_HANA_CONSTANT_CHECK(equal(
        foldr1(f, x<1>{}, x<2>{}, x<3>{},  x<4>{},  x<5>{},  x<6>{}, x<7>{},
                  x<8>{}, x<9>{}, x<10>{}, x<11>{}, x<12>{}, x<13>{}, x<14>{},
                  x<15>{}),
        f(x<1>{}, f(x<2>{}, f(x<3>{}, f(x<4>{}, f(x<5>{}, f(x<6>{}, f(x<7>{},
        f(x<8>{}, f(x<9>{}, f(x<10>{}, f(x<11>{}, f(x<12>{}, f(x<13>{}, f(x<14>{}, x<15>{}
        ))))))))))))))
    ));


    BOOST_HANA_CONSTANT_CHECK(equal(
        foldr1(f, x<1>{},  x<2>{},  x<3>{},  x<4>{},  x<5>{},  x<6>{},  x<7>{},
                  x<8>{},  x<9>{},  x<10>{}, x<11>{}, x<12>{}, x<13>{}, x<14>{},
                  x<15>{}, x<16>{}, x<17>{}, x<18>{}, x<19>{}, x<20>{}, x<21>{},
                  x<22>{}, x<23>{}, x<24>{}, x<25>{}, x<26>{}, x<27>{}),
        f(x<1>{},  f(x<2>{},  f(x<3>{},  f(x<4>{},  f(x<5>{},  f(x<6>{},  f(x<7>{},
        f(x<8>{},  f(x<9>{},  f(x<10>{}, f(x<11>{}, f(x<12>{}, f(x<13>{}, f(x<14>{},
        f(x<15>{}, f(x<16>{}, f(x<17>{}, f(x<18>{}, f(x<19>{}, f(x<20>{}, f(x<21>{},
        f(x<22>{}, f(x<23>{}, f(x<24>{}, f(x<25>{}, f(x<26>{}, x<27>{}
        ))))))))))))))))))))))))))
    ));

    BOOST_HANA_CONSTANT_CHECK(equal(
        foldr1(f, x<1>{},  x<2>{},  x<3>{},  x<4>{},  x<5>{},  x<6>{},  x<7>{},
                  x<8>{},  x<9>{},  x<10>{}, x<11>{}, x<12>{}, x<13>{}, x<14>{},
                  x<15>{}, x<16>{}, x<17>{}, x<18>{}, x<19>{}, x<20>{}, x<21>{},
                  x<22>{}, x<23>{}, x<24>{}, x<25>{}, x<26>{}, x<27>{}, x<28>{}),
        f(x<1>{},  f(x<2>{},  f(x<3>{},  f(x<4>{},  f(x<5>{},  f(x<6>{},  f(x<7>{},
        f(x<8>{},  f(x<9>{},  f(x<10>{}, f(x<11>{}, f(x<12>{}, f(x<13>{}, f(x<14>{},
        f(x<15>{}, f(x<16>{}, f(x<17>{}, f(x<18>{}, f(x<19>{}, f(x<20>{}, f(x<21>{},
        f(x<22>{}, f(x<23>{}, f(x<24>{}, f(x<25>{}, f(x<26>{}, f(x<27>{},   x<28>{}
        )))))))))))))))))))))))))))
    ));

    BOOST_HANA_CONSTANT_CHECK(equal(
        foldr1(f, x<1>{},  x<2>{},  x<3>{},  x<4>{},  x<5>{},  x<6>{},  x<7>{},
                  x<8>{},  x<9>{},  x<10>{}, x<11>{}, x<12>{}, x<13>{}, x<14>{},
                  x<15>{}, x<16>{}, x<17>{}, x<18>{}, x<19>{}, x<20>{}, x<21>{},
                  x<22>{}, x<23>{}, x<24>{}, x<25>{}, x<26>{}, x<27>{}, x<28>{},
                  x<29>{}),
        f(x<1>{},  f(x<2>{},  f(x<3>{},  f(x<4>{},  f(x<5>{},  f(x<6>{},  f(x<7>{},
        f(x<8>{},  f(x<9>{},  f(x<10>{}, f(x<11>{}, f(x<12>{}, f(x<13>{}, f(x<14>{},
        f(x<15>{}, f(x<16>{}, f(x<17>{}, f(x<18>{}, f(x<19>{}, f(x<20>{}, f(x<21>{},
        f(x<22>{}, f(x<23>{}, f(x<24>{}, f(x<25>{}, f(x<26>{}, f(x<27>{}, f(x<28>{},
          x<29>{}
        ))))))))))))))))))))))))))))
    ));


    BOOST_HANA_CONSTANT_CHECK(equal(
        foldr1(f, x<1>{},  x<2>{},  x<3>{},  x<4>{},  x<5>{},  x<6>{},  x<7>{},
                  x<8>{},  x<9>{},  x<10>{}, x<11>{}, x<12>{}, x<13>{}, x<14>{},
                  x<15>{}, x<16>{}, x<17>{}, x<18>{}, x<19>{}, x<20>{}, x<21>{},
                  x<22>{}, x<23>{}, x<24>{}, x<25>{}, x<26>{}, x<27>{}, x<28>{},
                  x<29>{}, x<30>{}, x<31>{}, x<32>{}, x<33>{}, x<34>{}, x<35>{},
                  x<36>{}, x<37>{}, x<38>{}, x<39>{}, x<40>{}, x<41>{}, x<42>{},
                  x<43>{}, x<44>{}, x<45>{}, x<46>{}, x<47>{}, x<48>{}, x<49>{},
                  x<50>{}, x<51>{}, x<52>{}, x<53>{}, x<54>{}, x<55>{}),
        f(x<1>{},  f(x<2>{},  f(x<3>{},  f(x<4>{},  f(x<5>{},  f(x<6>{},  f(x<7>{},
        f(x<8>{},  f(x<9>{},  f(x<10>{}, f(x<11>{}, f(x<12>{}, f(x<13>{}, f(x<14>{},
        f(x<15>{}, f(x<16>{}, f(x<17>{}, f(x<18>{}, f(x<19>{}, f(x<20>{}, f(x<21>{},
        f(x<22>{}, f(x<23>{}, f(x<24>{}, f(x<25>{}, f(x<26>{}, f(x<27>{}, f(x<28>{},
        f(x<29>{}, f(x<30>{}, f(x<31>{}, f(x<32>{}, f(x<33>{}, f(x<34>{}, f(x<35>{},
        f(x<36>{}, f(x<37>{}, f(x<38>{}, f(x<39>{}, f(x<40>{}, f(x<41>{}, f(x<42>{},
        f(x<43>{}, f(x<44>{}, f(x<45>{}, f(x<46>{}, f(x<47>{}, f(x<48>{}, f(x<49>{},
        f(x<50>{}, f(x<51>{}, f(x<52>{}, f(x<53>{}, f(x<54>{}, x<55>{}
        ))))))))))))))))))))))))))))))))))))))))))))))))))))))
    ));

    BOOST_HANA_CONSTANT_CHECK(equal(
        foldr1(f, x<1>{},  x<2>{},  x<3>{},  x<4>{},  x<5>{},  x<6>{},  x<7>{},
                  x<8>{},  x<9>{},  x<10>{}, x<11>{}, x<12>{}, x<13>{}, x<14>{},
                  x<15>{}, x<16>{}, x<17>{}, x<18>{}, x<19>{}, x<20>{}, x<21>{},
                  x<22>{}, x<23>{}, x<24>{}, x<25>{}, x<26>{}, x<27>{}, x<28>{},
                  x<29>{}, x<30>{}, x<31>{}, x<32>{}, x<33>{}, x<34>{}, x<35>{},
                  x<36>{}, x<37>{}, x<38>{}, x<39>{}, x<40>{}, x<41>{}, x<42>{},
                  x<43>{}, x<44>{}, x<45>{}, x<46>{}, x<47>{}, x<48>{}, x<49>{},
                  x<50>{}, x<51>{}, x<52>{}, x<53>{}, x<54>{}, x<55>{}, x<56>{}),
        f(x<1>{},  f(x<2>{},  f(x<3>{},  f(x<4>{},  f(x<5>{},  f(x<6>{},  f(x<7>{},
        f(x<8>{},  f(x<9>{},  f(x<10>{}, f(x<11>{}, f(x<12>{}, f(x<13>{}, f(x<14>{},
        f(x<15>{}, f(x<16>{}, f(x<17>{}, f(x<18>{}, f(x<19>{}, f(x<20>{}, f(x<21>{},
        f(x<22>{}, f(x<23>{}, f(x<24>{}, f(x<25>{}, f(x<26>{}, f(x<27>{}, f(x<28>{},
        f(x<29>{}, f(x<30>{}, f(x<31>{}, f(x<32>{}, f(x<33>{}, f(x<34>{}, f(x<35>{},
        f(x<36>{}, f(x<37>{}, f(x<38>{}, f(x<39>{}, f(x<40>{}, f(x<41>{}, f(x<42>{},
        f(x<43>{}, f(x<44>{}, f(x<45>{}, f(x<46>{}, f(x<47>{}, f(x<48>{}, f(x<49>{},
        f(x<50>{}, f(x<51>{}, f(x<52>{}, f(x<53>{}, f(x<54>{}, f(x<55>{},   x<56>{}
        )))))))))))))))))))))))))))))))))))))))))))))))))))))))
    ));

    BOOST_HANA_CONSTANT_CHECK(equal(
        foldr1(f, x<1>{},  x<2>{},  x<3>{},  x<4>{},  x<5>{},  x<6>{},  x<7>{},
                  x<8>{},  x<9>{},  x<10>{}, x<11>{}, x<12>{}, x<13>{}, x<14>{},
                  x<15>{}, x<16>{}, x<17>{}, x<18>{}, x<19>{}, x<20>{}, x<21>{},
                  x<22>{}, x<23>{}, x<24>{}, x<25>{}, x<26>{}, x<27>{}, x<28>{},
                  x<29>{}, x<30>{}, x<31>{}, x<32>{}, x<33>{}, x<34>{}, x<35>{},
                  x<36>{}, x<37>{}, x<38>{}, x<39>{}, x<40>{}, x<41>{}, x<42>{},
                  x<43>{}, x<44>{}, x<45>{}, x<46>{}, x<47>{}, x<48>{}, x<49>{},
                  x<50>{}, x<51>{}, x<52>{}, x<53>{}, x<54>{}, x<55>{}, x<56>{},
                  x<57>{}),
        f(x<1>{},  f(x<2>{},  f(x<3>{},  f(x<4>{},  f(x<5>{},  f(x<6>{},  f(x<7>{},
        f(x<8>{},  f(x<9>{},  f(x<10>{}, f(x<11>{}, f(x<12>{}, f(x<13>{}, f(x<14>{},
        f(x<15>{}, f(x<16>{}, f(x<17>{}, f(x<18>{}, f(x<19>{}, f(x<20>{}, f(x<21>{},
        f(x<22>{}, f(x<23>{}, f(x<24>{}, f(x<25>{}, f(x<26>{}, f(x<27>{}, f(x<28>{},
        f(x<29>{}, f(x<30>{}, f(x<31>{}, f(x<32>{}, f(x<33>{}, f(x<34>{}, f(x<35>{},
        f(x<36>{}, f(x<37>{}, f(x<38>{}, f(x<39>{}, f(x<40>{}, f(x<41>{}, f(x<42>{},
        f(x<43>{}, f(x<44>{}, f(x<45>{}, f(x<46>{}, f(x<47>{}, f(x<48>{}, f(x<49>{},
        f(x<50>{}, f(x<51>{}, f(x<52>{}, f(x<53>{}, f(x<54>{}, f(x<55>{}, f(x<56>{},
          x<57>{}
        ))))))))))))))))))))))))))))))))))))))))))))))))))))))))
    ));
}
