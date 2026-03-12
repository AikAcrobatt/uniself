
#include <iostream>
#include <vector>

//enum contents support

#define UNS_RENUM_OBTAIN_FIRST(first, ...) first
#define UNS_RENUM_MAKE_EQUALITY(arg1, arg2) arg1 arg2

#define UNS_RENUM_EXPAND(x) x
#define UNS_RENUM_FOR_FIRST(macro, ...) \
    UNS_RENUM_EXPAND(macro)UNS_RENUM_EXPAND(UNS_RENUM_OBTAIN_FIRST(__VA_ARGS__))

#define UNS_RENUM_GET_MACRO(_1, _2, _3, _4, _5, _6, _7, _8, _9, _10, _11, _12, _13, _14, _15, _16, _17, _18, _19, _20, _21, _22, _23, _24, _25, _26, _27, _28, _29, _30, _31, _32, _33, _34, _35, _36, _37, _38, _39, _40, _41, _42, _43, _44, _45, _46, _47, _48, _49, _50, _51, _52, _53, _54, _55, _56, _57, _58, _59, _60, _61, _62, _63, _64, _65, _66, _67, _68, _69, _70, _71, _72, _73, _74, _75, _76, _77, _78, _79, _80, _81, _82, _83, _84, _85, _86, _87, _88, _89, _90, _91, _92, _93, _94, _95, _96, _97, _98, _99, _100, _101, _102, _103, _104, _105, _106, _107, _108, _109, _110, _111, _112, _113, _114, _115, _116, _117, _118, _119, _120, _121, _122, _123, _124, _125, NAME, ...) NAME
#define UNS_RENUM_FOR_EACH(macro, delimiter, ...) \
    UNS_RENUM_EXPAND(UNS_RENUM_GET_MACRO(__VA_ARGS__, UNS_RENUM_FOR_125, UNS_RENUM_FOR_124, UNS_RENUM_FOR_123, UNS_RENUM_FOR_122, UNS_RENUM_FOR_121, UNS_RENUM_FOR_120, UNS_RENUM_FOR_119, UNS_RENUM_FOR_118, UNS_RENUM_FOR_117, UNS_RENUM_FOR_116, UNS_RENUM_FOR_115, UNS_RENUM_FOR_114, UNS_RENUM_FOR_113, UNS_RENUM_FOR_112, UNS_RENUM_FOR_111, UNS_RENUM_FOR_110, UNS_RENUM_FOR_109, UNS_RENUM_FOR_108, UNS_RENUM_FOR_107, UNS_RENUM_FOR_106, UNS_RENUM_FOR_105, UNS_RENUM_FOR_104, UNS_RENUM_FOR_103, UNS_RENUM_FOR_102, UNS_RENUM_FOR_101, UNS_RENUM_FOR_100, UNS_RENUM_FOR_99, UNS_RENUM_FOR_98, UNS_RENUM_FOR_97, UNS_RENUM_FOR_96, UNS_RENUM_FOR_95, UNS_RENUM_FOR_94, UNS_RENUM_FOR_93, UNS_RENUM_FOR_92, UNS_RENUM_FOR_91, UNS_RENUM_FOR_90, UNS_RENUM_FOR_89, UNS_RENUM_FOR_88, UNS_RENUM_FOR_87, UNS_RENUM_FOR_86, UNS_RENUM_FOR_85, UNS_RENUM_FOR_84, UNS_RENUM_FOR_83, UNS_RENUM_FOR_82, UNS_RENUM_FOR_81, UNS_RENUM_FOR_80, UNS_RENUM_FOR_79, UNS_RENUM_FOR_78, UNS_RENUM_FOR_77, UNS_RENUM_FOR_76, UNS_RENUM_FOR_75, UNS_RENUM_FOR_74, UNS_RENUM_FOR_73, UNS_RENUM_FOR_72, UNS_RENUM_FOR_71, UNS_RENUM_FOR_70, UNS_RENUM_FOR_69, UNS_RENUM_FOR_68, UNS_RENUM_FOR_67, UNS_RENUM_FOR_66, UNS_RENUM_FOR_65, UNS_RENUM_FOR_64, UNS_RENUM_FOR_63, UNS_RENUM_FOR_62, UNS_RENUM_FOR_61, UNS_RENUM_FOR_60, UNS_RENUM_FOR_59, UNS_RENUM_FOR_58, UNS_RENUM_FOR_57, UNS_RENUM_FOR_56, UNS_RENUM_FOR_55, UNS_RENUM_FOR_54, UNS_RENUM_FOR_53, UNS_RENUM_FOR_52, UNS_RENUM_FOR_51, UNS_RENUM_FOR_50, UNS_RENUM_FOR_49, UNS_RENUM_FOR_48, UNS_RENUM_FOR_47, UNS_RENUM_FOR_46, UNS_RENUM_FOR_45, UNS_RENUM_FOR_44, UNS_RENUM_FOR_43, UNS_RENUM_FOR_42, UNS_RENUM_FOR_41, UNS_RENUM_FOR_40, UNS_RENUM_FOR_39, UNS_RENUM_FOR_38, UNS_RENUM_FOR_37, UNS_RENUM_FOR_36, UNS_RENUM_FOR_35, UNS_RENUM_FOR_34, UNS_RENUM_FOR_33, UNS_RENUM_FOR_32, UNS_RENUM_FOR_31, UNS_RENUM_FOR_30, UNS_RENUM_FOR_29, UNS_RENUM_FOR_28, UNS_RENUM_FOR_27, UNS_RENUM_FOR_26, UNS_RENUM_FOR_25, UNS_RENUM_FOR_24, UNS_RENUM_FOR_23, UNS_RENUM_FOR_22, UNS_RENUM_FOR_21, UNS_RENUM_FOR_20, UNS_RENUM_FOR_19, UNS_RENUM_FOR_18, UNS_RENUM_FOR_17, UNS_RENUM_FOR_16, UNS_RENUM_FOR_15, UNS_RENUM_FOR_14, UNS_RENUM_FOR_13, UNS_RENUM_FOR_12, UNS_RENUM_FOR_11, UNS_RENUM_FOR_10, UNS_RENUM_FOR_9, UNS_RENUM_FOR_8, UNS_RENUM_FOR_7, UNS_RENUM_FOR_6, UNS_RENUM_FOR_5, UNS_RENUM_FOR_4, UNS_RENUM_FOR_3, UNS_RENUM_FOR_2, UNS_RENUM_FOR_1)(macro, delimiter, __VA_ARGS__))

#define UNS_RENUM_FOR_1(m, d, x) m(x)
#define UNS_RENUM_FOR_2(m, d, x, ...) m(x) UNS_RENUM_EXPAND(d()) UNS_RENUM_EXPAND(UNS_RENUM_FOR_1(m, d, __VA_ARGS__))
#define UNS_RENUM_FOR_3(m, d, x, ...) m(x) UNS_RENUM_EXPAND(d()) UNS_RENUM_EXPAND(UNS_RENUM_FOR_2(m, d, __VA_ARGS__))
#define UNS_RENUM_FOR_4(m, d, x, ...) m(x) UNS_RENUM_EXPAND(d()) UNS_RENUM_EXPAND(UNS_RENUM_FOR_3(m, d, __VA_ARGS__))
#define UNS_RENUM_FOR_5(m, d, x, ...) m(x) UNS_RENUM_EXPAND(d()) UNS_RENUM_EXPAND(UNS_RENUM_FOR_4(m, d, __VA_ARGS__))
#define UNS_RENUM_FOR_6(m, d, x, ...) m(x) UNS_RENUM_EXPAND(d()) UNS_RENUM_EXPAND(UNS_RENUM_FOR_5(m, d, __VA_ARGS__))
#define UNS_RENUM_FOR_7(m, d, x, ...) m(x) UNS_RENUM_EXPAND(d()) UNS_RENUM_EXPAND(UNS_RENUM_FOR_6(m, d, __VA_ARGS__))
#define UNS_RENUM_FOR_8(m, d, x, ...) m(x) UNS_RENUM_EXPAND(d()) UNS_RENUM_EXPAND(UNS_RENUM_FOR_7(m, d, __VA_ARGS__))
#define UNS_RENUM_FOR_9(m, d, x, ...) m(x) UNS_RENUM_EXPAND(d()) UNS_RENUM_EXPAND(UNS_RENUM_FOR_8(m, d, __VA_ARGS__))
#define UNS_RENUM_FOR_10(m, d, x, ...) m(x) UNS_RENUM_EXPAND(d()) UNS_RENUM_EXPAND(UNS_RENUM_FOR_9(m, d, __VA_ARGS__))
#define UNS_RENUM_FOR_11(m, d, x, ...) m(x) UNS_RENUM_EXPAND(d()) UNS_RENUM_EXPAND(UNS_RENUM_FOR_10(m, d, __VA_ARGS__))
#define UNS_RENUM_FOR_12(m, d, x, ...) m(x) UNS_RENUM_EXPAND(d()) UNS_RENUM_EXPAND(UNS_RENUM_FOR_11(m, d, __VA_ARGS__))
#define UNS_RENUM_FOR_13(m, d, x, ...) m(x) UNS_RENUM_EXPAND(d()) UNS_RENUM_EXPAND(UNS_RENUM_FOR_12(m, d, __VA_ARGS__))
#define UNS_RENUM_FOR_14(m, d, x, ...) m(x) UNS_RENUM_EXPAND(d()) UNS_RENUM_EXPAND(UNS_RENUM_FOR_13(m, d, __VA_ARGS__))
#define UNS_RENUM_FOR_15(m, d, x, ...) m(x) UNS_RENUM_EXPAND(d()) UNS_RENUM_EXPAND(UNS_RENUM_FOR_14(m, d, __VA_ARGS__))
#define UNS_RENUM_FOR_16(m, d, x, ...) m(x) UNS_RENUM_EXPAND(d()) UNS_RENUM_EXPAND(UNS_RENUM_FOR_15(m, d, __VA_ARGS__))
#define UNS_RENUM_FOR_17(m, d, x, ...) m(x) UNS_RENUM_EXPAND(d()) UNS_RENUM_EXPAND(UNS_RENUM_FOR_16(m, d, __VA_ARGS__))
#define UNS_RENUM_FOR_18(m, d, x, ...) m(x) UNS_RENUM_EXPAND(d()) UNS_RENUM_EXPAND(UNS_RENUM_FOR_17(m, d, __VA_ARGS__))
#define UNS_RENUM_FOR_19(m, d, x, ...) m(x) UNS_RENUM_EXPAND(d()) UNS_RENUM_EXPAND(UNS_RENUM_FOR_18(m, d, __VA_ARGS__))
#define UNS_RENUM_FOR_20(m, d, x, ...) m(x) UNS_RENUM_EXPAND(d()) UNS_RENUM_EXPAND(UNS_RENUM_FOR_19(m, d, __VA_ARGS__))
#define UNS_RENUM_FOR_21(m, d, x, ...) m(x) UNS_RENUM_EXPAND(d()) UNS_RENUM_EXPAND(UNS_RENUM_FOR_20(m, d, __VA_ARGS__))
#define UNS_RENUM_FOR_22(m, d, x, ...) m(x) UNS_RENUM_EXPAND(d()) UNS_RENUM_EXPAND(UNS_RENUM_FOR_21(m, d, __VA_ARGS__))
#define UNS_RENUM_FOR_23(m, d, x, ...) m(x) UNS_RENUM_EXPAND(d()) UNS_RENUM_EXPAND(UNS_RENUM_FOR_22(m, d, __VA_ARGS__))
#define UNS_RENUM_FOR_24(m, d, x, ...) m(x) UNS_RENUM_EXPAND(d()) UNS_RENUM_EXPAND(UNS_RENUM_FOR_23(m, d, __VA_ARGS__))
#define UNS_RENUM_FOR_25(m, d, x, ...) m(x) UNS_RENUM_EXPAND(d()) UNS_RENUM_EXPAND(UNS_RENUM_FOR_24(m, d, __VA_ARGS__))
#define UNS_RENUM_FOR_26(m, d, x, ...) m(x) UNS_RENUM_EXPAND(d()) UNS_RENUM_EXPAND(UNS_RENUM_FOR_25(m, d, __VA_ARGS__))
#define UNS_RENUM_FOR_27(m, d, x, ...) m(x) UNS_RENUM_EXPAND(d()) UNS_RENUM_EXPAND(UNS_RENUM_FOR_26(m, d, __VA_ARGS__))
#define UNS_RENUM_FOR_28(m, d, x, ...) m(x) UNS_RENUM_EXPAND(d()) UNS_RENUM_EXPAND(UNS_RENUM_FOR_27(m, d, __VA_ARGS__))
#define UNS_RENUM_FOR_29(m, d, x, ...) m(x) UNS_RENUM_EXPAND(d()) UNS_RENUM_EXPAND(UNS_RENUM_FOR_28(m, d, __VA_ARGS__))
#define UNS_RENUM_FOR_30(m, d, x, ...) m(x) UNS_RENUM_EXPAND(d()) UNS_RENUM_EXPAND(UNS_RENUM_FOR_29(m, d, __VA_ARGS__))
#define UNS_RENUM_FOR_31(m, d, x, ...) m(x) UNS_RENUM_EXPAND(d()) UNS_RENUM_EXPAND(UNS_RENUM_FOR_30(m, d, __VA_ARGS__))
#define UNS_RENUM_FOR_32(m, d, x, ...) m(x) UNS_RENUM_EXPAND(d()) UNS_RENUM_EXPAND(UNS_RENUM_FOR_31(m, d, __VA_ARGS__))
#define UNS_RENUM_FOR_33(m, d, x, ...) m(x) UNS_RENUM_EXPAND(d()) UNS_RENUM_EXPAND(UNS_RENUM_FOR_32(m, d, __VA_ARGS__))
#define UNS_RENUM_FOR_34(m, d, x, ...) m(x) UNS_RENUM_EXPAND(d()) UNS_RENUM_EXPAND(UNS_RENUM_FOR_33(m, d, __VA_ARGS__))
#define UNS_RENUM_FOR_35(m, d, x, ...) m(x) UNS_RENUM_EXPAND(d()) UNS_RENUM_EXPAND(UNS_RENUM_FOR_34(m, d, __VA_ARGS__))
#define UNS_RENUM_FOR_36(m, d, x, ...) m(x) UNS_RENUM_EXPAND(d()) UNS_RENUM_EXPAND(UNS_RENUM_FOR_35(m, d, __VA_ARGS__))
#define UNS_RENUM_FOR_37(m, d, x, ...) m(x) UNS_RENUM_EXPAND(d()) UNS_RENUM_EXPAND(UNS_RENUM_FOR_36(m, d, __VA_ARGS__))
#define UNS_RENUM_FOR_38(m, d, x, ...) m(x) UNS_RENUM_EXPAND(d()) UNS_RENUM_EXPAND(UNS_RENUM_FOR_37(m, d, __VA_ARGS__))
#define UNS_RENUM_FOR_39(m, d, x, ...) m(x) UNS_RENUM_EXPAND(d()) UNS_RENUM_EXPAND(UNS_RENUM_FOR_38(m, d, __VA_ARGS__))
#define UNS_RENUM_FOR_40(m, d, x, ...) m(x) UNS_RENUM_EXPAND(d()) UNS_RENUM_EXPAND(UNS_RENUM_FOR_39(m, d, __VA_ARGS__))
#define UNS_RENUM_FOR_41(m, d, x, ...) m(x) UNS_RENUM_EXPAND(d()) UNS_RENUM_EXPAND(UNS_RENUM_FOR_40(m, d, __VA_ARGS__))
#define UNS_RENUM_FOR_42(m, d, x, ...) m(x) UNS_RENUM_EXPAND(d()) UNS_RENUM_EXPAND(UNS_RENUM_FOR_41(m, d, __VA_ARGS__))
#define UNS_RENUM_FOR_43(m, d, x, ...) m(x) UNS_RENUM_EXPAND(d()) UNS_RENUM_EXPAND(UNS_RENUM_FOR_42(m, d, __VA_ARGS__))
#define UNS_RENUM_FOR_44(m, d, x, ...) m(x) UNS_RENUM_EXPAND(d()) UNS_RENUM_EXPAND(UNS_RENUM_FOR_43(m, d, __VA_ARGS__))
#define UNS_RENUM_FOR_45(m, d, x, ...) m(x) UNS_RENUM_EXPAND(d()) UNS_RENUM_EXPAND(UNS_RENUM_FOR_44(m, d, __VA_ARGS__))
#define UNS_RENUM_FOR_46(m, d, x, ...) m(x) UNS_RENUM_EXPAND(d()) UNS_RENUM_EXPAND(UNS_RENUM_FOR_45(m, d, __VA_ARGS__))
#define UNS_RENUM_FOR_47(m, d, x, ...) m(x) UNS_RENUM_EXPAND(d()) UNS_RENUM_EXPAND(UNS_RENUM_FOR_46(m, d, __VA_ARGS__))
#define UNS_RENUM_FOR_48(m, d, x, ...) m(x) UNS_RENUM_EXPAND(d()) UNS_RENUM_EXPAND(UNS_RENUM_FOR_47(m, d, __VA_ARGS__))
#define UNS_RENUM_FOR_49(m, d, x, ...) m(x) UNS_RENUM_EXPAND(d()) UNS_RENUM_EXPAND(UNS_RENUM_FOR_48(m, d, __VA_ARGS__))
#define UNS_RENUM_FOR_50(m, d, x, ...) m(x) UNS_RENUM_EXPAND(d()) UNS_RENUM_EXPAND(UNS_RENUM_FOR_49(m, d, __VA_ARGS__))
#define UNS_RENUM_FOR_51(m, d, x, ...) m(x) UNS_RENUM_EXPAND(d()) UNS_RENUM_EXPAND(UNS_RENUM_FOR_50(m, d, __VA_ARGS__))
#define UNS_RENUM_FOR_52(m, d, x, ...) m(x) UNS_RENUM_EXPAND(d()) UNS_RENUM_EXPAND(UNS_RENUM_FOR_51(m, d, __VA_ARGS__))
#define UNS_RENUM_FOR_53(m, d, x, ...) m(x) UNS_RENUM_EXPAND(d()) UNS_RENUM_EXPAND(UNS_RENUM_FOR_52(m, d, __VA_ARGS__))
#define UNS_RENUM_FOR_54(m, d, x, ...) m(x) UNS_RENUM_EXPAND(d()) UNS_RENUM_EXPAND(UNS_RENUM_FOR_53(m, d, __VA_ARGS__))
#define UNS_RENUM_FOR_55(m, d, x, ...) m(x) UNS_RENUM_EXPAND(d()) UNS_RENUM_EXPAND(UNS_RENUM_FOR_54(m, d, __VA_ARGS__))
#define UNS_RENUM_FOR_56(m, d, x, ...) m(x) UNS_RENUM_EXPAND(d()) UNS_RENUM_EXPAND(UNS_RENUM_FOR_55(m, d, __VA_ARGS__))
#define UNS_RENUM_FOR_57(m, d, x, ...) m(x) UNS_RENUM_EXPAND(d()) UNS_RENUM_EXPAND(UNS_RENUM_FOR_56(m, d, __VA_ARGS__))
#define UNS_RENUM_FOR_58(m, d, x, ...) m(x) UNS_RENUM_EXPAND(d()) UNS_RENUM_EXPAND(UNS_RENUM_FOR_57(m, d, __VA_ARGS__))
#define UNS_RENUM_FOR_59(m, d, x, ...) m(x) UNS_RENUM_EXPAND(d()) UNS_RENUM_EXPAND(UNS_RENUM_FOR_58(m, d, __VA_ARGS__))
#define UNS_RENUM_FOR_60(m, d, x, ...) m(x) UNS_RENUM_EXPAND(d()) UNS_RENUM_EXPAND(UNS_RENUM_FOR_59(m, d, __VA_ARGS__))
#define UNS_RENUM_FOR_61(m, d, x, ...) m(x) UNS_RENUM_EXPAND(d()) UNS_RENUM_EXPAND(UNS_RENUM_FOR_60(m, d, __VA_ARGS__))
#define UNS_RENUM_FOR_62(m, d, x, ...) m(x) UNS_RENUM_EXPAND(d()) UNS_RENUM_EXPAND(UNS_RENUM_FOR_61(m, d, __VA_ARGS__))
#define UNS_RENUM_FOR_63(m, d, x, ...) m(x) UNS_RENUM_EXPAND(d()) UNS_RENUM_EXPAND(UNS_RENUM_FOR_62(m, d, __VA_ARGS__))
#define UNS_RENUM_FOR_64(m, d, x, ...) m(x) UNS_RENUM_EXPAND(d()) UNS_RENUM_EXPAND(UNS_RENUM_FOR_63(m, d, __VA_ARGS__))
#define UNS_RENUM_FOR_65(m, d, x, ...) m(x) UNS_RENUM_EXPAND(d()) UNS_RENUM_EXPAND(UNS_RENUM_FOR_64(m, d, __VA_ARGS__))
#define UNS_RENUM_FOR_66(m, d, x, ...) m(x) UNS_RENUM_EXPAND(d()) UNS_RENUM_EXPAND(UNS_RENUM_FOR_65(m, d, __VA_ARGS__))
#define UNS_RENUM_FOR_67(m, d, x, ...) m(x) UNS_RENUM_EXPAND(d()) UNS_RENUM_EXPAND(UNS_RENUM_FOR_66(m, d, __VA_ARGS__))
#define UNS_RENUM_FOR_68(m, d, x, ...) m(x) UNS_RENUM_EXPAND(d()) UNS_RENUM_EXPAND(UNS_RENUM_FOR_67(m, d, __VA_ARGS__))
#define UNS_RENUM_FOR_69(m, d, x, ...) m(x) UNS_RENUM_EXPAND(d()) UNS_RENUM_EXPAND(UNS_RENUM_FOR_68(m, d, __VA_ARGS__))
#define UNS_RENUM_FOR_70(m, d, x, ...) m(x) UNS_RENUM_EXPAND(d()) UNS_RENUM_EXPAND(UNS_RENUM_FOR_69(m, d, __VA_ARGS__))
#define UNS_RENUM_FOR_71(m, d, x, ...) m(x) UNS_RENUM_EXPAND(d()) UNS_RENUM_EXPAND(UNS_RENUM_FOR_70(m, d, __VA_ARGS__))
#define UNS_RENUM_FOR_72(m, d, x, ...) m(x) UNS_RENUM_EXPAND(d()) UNS_RENUM_EXPAND(UNS_RENUM_FOR_71(m, d, __VA_ARGS__))
#define UNS_RENUM_FOR_73(m, d, x, ...) m(x) UNS_RENUM_EXPAND(d()) UNS_RENUM_EXPAND(UNS_RENUM_FOR_72(m, d, __VA_ARGS__))
#define UNS_RENUM_FOR_74(m, d, x, ...) m(x) UNS_RENUM_EXPAND(d()) UNS_RENUM_EXPAND(UNS_RENUM_FOR_73(m, d, __VA_ARGS__))
#define UNS_RENUM_FOR_75(m, d, x, ...) m(x) UNS_RENUM_EXPAND(d()) UNS_RENUM_EXPAND(UNS_RENUM_FOR_74(m, d, __VA_ARGS__))
#define UNS_RENUM_FOR_76(m, d, x, ...) m(x) UNS_RENUM_EXPAND(d()) UNS_RENUM_EXPAND(UNS_RENUM_FOR_75(m, d, __VA_ARGS__))
#define UNS_RENUM_FOR_77(m, d, x, ...) m(x) UNS_RENUM_EXPAND(d()) UNS_RENUM_EXPAND(UNS_RENUM_FOR_76(m, d, __VA_ARGS__))
#define UNS_RENUM_FOR_78(m, d, x, ...) m(x) UNS_RENUM_EXPAND(d()) UNS_RENUM_EXPAND(UNS_RENUM_FOR_77(m, d, __VA_ARGS__))
#define UNS_RENUM_FOR_79(m, d, x, ...) m(x) UNS_RENUM_EXPAND(d()) UNS_RENUM_EXPAND(UNS_RENUM_FOR_78(m, d, __VA_ARGS__))
#define UNS_RENUM_FOR_80(m, d, x, ...) m(x) UNS_RENUM_EXPAND(d()) UNS_RENUM_EXPAND(UNS_RENUM_FOR_79(m, d, __VA_ARGS__))
#define UNS_RENUM_FOR_81(m, d, x, ...) m(x) UNS_RENUM_EXPAND(d()) UNS_RENUM_EXPAND(UNS_RENUM_FOR_80(m, d, __VA_ARGS__))
#define UNS_RENUM_FOR_82(m, d, x, ...) m(x) UNS_RENUM_EXPAND(d()) UNS_RENUM_EXPAND(UNS_RENUM_FOR_81(m, d, __VA_ARGS__))
#define UNS_RENUM_FOR_83(m, d, x, ...) m(x) UNS_RENUM_EXPAND(d()) UNS_RENUM_EXPAND(UNS_RENUM_FOR_82(m, d, __VA_ARGS__))
#define UNS_RENUM_FOR_84(m, d, x, ...) m(x) UNS_RENUM_EXPAND(d()) UNS_RENUM_EXPAND(UNS_RENUM_FOR_83(m, d, __VA_ARGS__))
#define UNS_RENUM_FOR_85(m, d, x, ...) m(x) UNS_RENUM_EXPAND(d()) UNS_RENUM_EXPAND(UNS_RENUM_FOR_84(m, d, __VA_ARGS__))
#define UNS_RENUM_FOR_86(m, d, x, ...) m(x) UNS_RENUM_EXPAND(d()) UNS_RENUM_EXPAND(UNS_RENUM_FOR_85(m, d, __VA_ARGS__))
#define UNS_RENUM_FOR_87(m, d, x, ...) m(x) UNS_RENUM_EXPAND(d()) UNS_RENUM_EXPAND(UNS_RENUM_FOR_86(m, d, __VA_ARGS__))
#define UNS_RENUM_FOR_88(m, d, x, ...) m(x) UNS_RENUM_EXPAND(d()) UNS_RENUM_EXPAND(UNS_RENUM_FOR_87(m, d, __VA_ARGS__))
#define UNS_RENUM_FOR_89(m, d, x, ...) m(x) UNS_RENUM_EXPAND(d()) UNS_RENUM_EXPAND(UNS_RENUM_FOR_88(m, d, __VA_ARGS__))
#define UNS_RENUM_FOR_90(m, d, x, ...) m(x) UNS_RENUM_EXPAND(d()) UNS_RENUM_EXPAND(UNS_RENUM_FOR_89(m, d, __VA_ARGS__))
#define UNS_RENUM_FOR_91(m, d, x, ...) m(x) UNS_RENUM_EXPAND(d()) UNS_RENUM_EXPAND(UNS_RENUM_FOR_90(m, d, __VA_ARGS__))
#define UNS_RENUM_FOR_92(m, d, x, ...) m(x) UNS_RENUM_EXPAND(d()) UNS_RENUM_EXPAND(UNS_RENUM_FOR_91(m, d, __VA_ARGS__))
#define UNS_RENUM_FOR_93(m, d, x, ...) m(x) UNS_RENUM_EXPAND(d()) UNS_RENUM_EXPAND(UNS_RENUM_FOR_92(m, d, __VA_ARGS__))
#define UNS_RENUM_FOR_94(m, d, x, ...) m(x) UNS_RENUM_EXPAND(d()) UNS_RENUM_EXPAND(UNS_RENUM_FOR_93(m, d, __VA_ARGS__))
#define UNS_RENUM_FOR_95(m, d, x, ...) m(x) UNS_RENUM_EXPAND(d()) UNS_RENUM_EXPAND(UNS_RENUM_FOR_94(m, d, __VA_ARGS__))
#define UNS_RENUM_FOR_96(m, d, x, ...) m(x) UNS_RENUM_EXPAND(d()) UNS_RENUM_EXPAND(UNS_RENUM_FOR_95(m, d, __VA_ARGS__))
#define UNS_RENUM_FOR_97(m, d, x, ...) m(x) UNS_RENUM_EXPAND(d()) UNS_RENUM_EXPAND(UNS_RENUM_FOR_96(m, d, __VA_ARGS__))
#define UNS_RENUM_FOR_98(m, d, x, ...) m(x) UNS_RENUM_EXPAND(d()) UNS_RENUM_EXPAND(UNS_RENUM_FOR_97(m, d, __VA_ARGS__))
#define UNS_RENUM_FOR_99(m, d, x, ...) m(x) UNS_RENUM_EXPAND(d()) UNS_RENUM_EXPAND(UNS_RENUM_FOR_98(m, d, __VA_ARGS__))
#define UNS_RENUM_FOR_100(m, d, x, ...) m(x) UNS_RENUM_EXPAND(d()) UNS_RENUM_EXPAND(UNS_RENUM_FOR_99(m, d, __VA_ARGS__))
#define UNS_RENUM_FOR_101(m, d, x, ...) m(x) UNS_RENUM_EXPAND(d()) UNS_RENUM_EXPAND(UNS_RENUM_FOR_100(m, d, __VA_ARGS__))
#define UNS_RENUM_FOR_102(m, d, x, ...) m(x) UNS_RENUM_EXPAND(d()) UNS_RENUM_EXPAND(UNS_RENUM_FOR_101(m, d, __VA_ARGS__))
#define UNS_RENUM_FOR_103(m, d, x, ...) m(x) UNS_RENUM_EXPAND(d()) UNS_RENUM_EXPAND(UNS_RENUM_FOR_102(m, d, __VA_ARGS__))
#define UNS_RENUM_FOR_104(m, d, x, ...) m(x) UNS_RENUM_EXPAND(d()) UNS_RENUM_EXPAND(UNS_RENUM_FOR_103(m, d, __VA_ARGS__))
#define UNS_RENUM_FOR_105(m, d, x, ...) m(x) UNS_RENUM_EXPAND(d()) UNS_RENUM_EXPAND(UNS_RENUM_FOR_104(m, d, __VA_ARGS__))
#define UNS_RENUM_FOR_106(m, d, x, ...) m(x) UNS_RENUM_EXPAND(d()) UNS_RENUM_EXPAND(UNS_RENUM_FOR_105(m, d, __VA_ARGS__))
#define UNS_RENUM_FOR_107(m, d, x, ...) m(x) UNS_RENUM_EXPAND(d()) UNS_RENUM_EXPAND(UNS_RENUM_FOR_106(m, d, __VA_ARGS__))
#define UNS_RENUM_FOR_108(m, d, x, ...) m(x) UNS_RENUM_EXPAND(d()) UNS_RENUM_EXPAND(UNS_RENUM_FOR_107(m, d, __VA_ARGS__))
#define UNS_RENUM_FOR_109(m, d, x, ...) m(x) UNS_RENUM_EXPAND(d()) UNS_RENUM_EXPAND(UNS_RENUM_FOR_108(m, d, __VA_ARGS__))
#define UNS_RENUM_FOR_110(m, d, x, ...) m(x) UNS_RENUM_EXPAND(d()) UNS_RENUM_EXPAND(UNS_RENUM_FOR_109(m, d, __VA_ARGS__))
#define UNS_RENUM_FOR_111(m, d, x, ...) m(x) UNS_RENUM_EXPAND(d()) UNS_RENUM_EXPAND(UNS_RENUM_FOR_110(m, d, __VA_ARGS__))
#define UNS_RENUM_FOR_112(m, d, x, ...) m(x) UNS_RENUM_EXPAND(d()) UNS_RENUM_EXPAND(UNS_RENUM_FOR_111(m, d, __VA_ARGS__))
#define UNS_RENUM_FOR_113(m, d, x, ...) m(x) UNS_RENUM_EXPAND(d()) UNS_RENUM_EXPAND(UNS_RENUM_FOR_112(m, d, __VA_ARGS__))
#define UNS_RENUM_FOR_114(m, d, x, ...) m(x) UNS_RENUM_EXPAND(d()) UNS_RENUM_EXPAND(UNS_RENUM_FOR_113(m, d, __VA_ARGS__))
#define UNS_RENUM_FOR_115(m, d, x, ...) m(x) UNS_RENUM_EXPAND(d()) UNS_RENUM_EXPAND(UNS_RENUM_FOR_114(m, d, __VA_ARGS__))
#define UNS_RENUM_FOR_116(m, d, x, ...) m(x) UNS_RENUM_EXPAND(d()) UNS_RENUM_EXPAND(UNS_RENUM_FOR_115(m, d, __VA_ARGS__))
#define UNS_RENUM_FOR_117(m, d, x, ...) m(x) UNS_RENUM_EXPAND(d()) UNS_RENUM_EXPAND(UNS_RENUM_FOR_116(m, d, __VA_ARGS__))
#define UNS_RENUM_FOR_118(m, d, x, ...) m(x) UNS_RENUM_EXPAND(d()) UNS_RENUM_EXPAND(UNS_RENUM_FOR_117(m, d, __VA_ARGS__))
#define UNS_RENUM_FOR_119(m, d, x, ...) m(x) UNS_RENUM_EXPAND(d()) UNS_RENUM_EXPAND(UNS_RENUM_FOR_118(m, d, __VA_ARGS__))
#define UNS_RENUM_FOR_120(m, d, x, ...) m(x) UNS_RENUM_EXPAND(d()) UNS_RENUM_EXPAND(UNS_RENUM_FOR_119(m, d, __VA_ARGS__))
#define UNS_RENUM_FOR_121(m, d, x, ...) m(x) UNS_RENUM_EXPAND(d()) UNS_RENUM_EXPAND(UNS_RENUM_FOR_120(m, d, __VA_ARGS__))
#define UNS_RENUM_FOR_122(m, d, x, ...) m(x) UNS_RENUM_EXPAND(d()) UNS_RENUM_EXPAND(UNS_RENUM_FOR_121(m, d, __VA_ARGS__))
#define UNS_RENUM_FOR_123(m, d, x, ...) m(x) UNS_RENUM_EXPAND(d()) UNS_RENUM_EXPAND(UNS_RENUM_FOR_122(m, d, __VA_ARGS__))
#define UNS_RENUM_FOR_124(m, d, x, ...) m(x) UNS_RENUM_EXPAND(d()) UNS_RENUM_EXPAND(UNS_RENUM_FOR_123(m, d, __VA_ARGS__))

#define UNS_RENUM_COUNTER(...)\
    UNS_RENUM_EXPAND(UNS_RENUM_GET_MACRO(__VA_ARGS__, 125, 124, 123, 122, 121, 120, 119, 118, 117, 116, 115, 114, 113, 112, 111, 110, 109, 108, 107, 106, 105, 104, 103, 102, 101, 100, 99, 98, 97, 96, 95, 94, 93, 92, 91, 90, 89, 88, 87, 86, 85, 84, 83, 82, 81, 80, 79, 78, 77, 76, 75, 74, 73, 72, 71, 70, 69, 68, 67, 66, 65, 64, 63, 62, 61, 60, 59, 58, 57, 56, 55, 54, 53, 52, 51, 50, 49, 48, 47, 46, 45, 44, 43, 42, 41, 40, 39, 38, 37, 36, 35, 34, 33, 32, 31, 30, 29, 28, 27, 26, 25, 24, 23, 22, 21, 20, 19, 18, 17, 16, 15, 14, 13, 12, 11, 10, 9, 8, 7, 6, 5, 4, 3, 2, 1))

//renum-specific macros

#define UNS_U8INSTRUCTION(arg) u8#arg
#define UNS_U8LITERAL(arg) UNS_U8INSTRUCTION(##arg)

#define UNS_RENUM_DELIMITER_NONE()
#define UNS_RENUM_DELIMITER_COMMA() ,

#define UNS_RENUM_SEGMENT(renum_pair) UNS_RENUM_EXPAND(UNS_RENUM_MAKE_EQUALITY)renum_pair
#define UNS_RENUM_VALUE(renum_pair) UNS_RENUM_EXPAND(UNS_RENUM_OBTAIN_FIRST)renum_pair

#define UNS_RENUM_TOSTRING_SEGMENT_INNER(arg)\
    case arg: { return UNS_U8LITERAL(arg); }
#define UNS_RENUM_TOSTRING_SEGMENT(renum_pair)\
    UNS_RENUM_TOSTRING_SEGMENT_INNER(UNS_RENUM_EXPAND(UNS_RENUM_OBTAIN_FIRST)renum_pair)

#define UNS_RENUM_FROMSTRING_SEGMENT_INNER(arg)\
    if (Str == UNS_U8LITERAL(arg)) { return arg; } else
#define UNS_RENUM_FROMSTRING_SEGMENT(renum_pair)\
    UNS_RENUM_FROMSTRING_SEGMENT_INNER(UNS_RENUM_EXPAND(UNS_RENUM_OBTAIN_FIRST)renum_pair)

#define UNS_RENUM(renum_name, underlying_type, ...)\
class renum_name {\
public:\
    using integral_type = underlying_type;\
public:\
    enum enum_type : integral_type {\
        UNS_RENUM_FOR_EACH(\
            UNS_RENUM_SEGMENT,\
            UNS_RENUM_DELIMITER_COMMA,\
            __VA_ARGS__\
        )\
    };\
    inline static constexpr ::std::size_t s_size = UNS_RENUM_COUNTER(\
        __VA_ARGS__\
    );\
protected:\
    enum_type m_value = UNS_RENUM_FOR_FIRST(\
        UNS_RENUM_OBTAIN_FIRST,\
        __VA_ARGS__\
    );\
public:\
    inline constexpr renum_name() noexcept {};\
    inline constexpr renum_name(enum_type EnumVal) noexcept : m_value{ EnumVal } {};\
    inline constexpr renum_name(const renum_name& Obj) noexcept : m_value{ Obj.m_value } {};\
    inline constexpr renum_name& operator=(const renum_name& Obj) noexcept {\
        if (this == &Obj) return *this;\
        m_value = Obj.m_value;\
        return *this;\
    };\
    inline constexpr renum_name(renum_name&& Obj) noexcept : m_value{ ::std::move(Obj.m_value) } {};\
    inline constexpr renum_name& operator=(renum_name&& Obj) noexcept {\
        if (this == &Obj) return *this;\
        m_value = ::std::move(Obj.m_value);\
        return *this;\
    };\
    inline constexpr ~renum_name() noexcept {};\
public:\
    inline constexpr bool operator==(const renum_name& arg) const noexcept { return m_value == arg.m_value; };\
    inline constexpr bool operator!=(const renum_name& arg) const noexcept { return !(m_value == arg.m_value); };\
public:\
    inline explicit constexpr operator enum_type() const noexcept { return m_value; };\
    inline explicit constexpr operator integral_type() const noexcept { return m_value; };\
public:\
    inline constexpr static ::std::size_t size() noexcept { return s_size; };\
    inline constexpr static ::std::vector<renum_name> values() {/*TODO*/\
        return {\
            UNS_RENUM_FOR_EACH(\
                UNS_RENUM_VALUE,\
                UNS_RENUM_DELIMITER_COMMA,\
                __VA_ARGS__\
            )\
        };\
    };\
public:\
    inline constexpr ::std::u8string to_string() const {\
        switch (m_value) {\
            UNS_RENUM_FOR_EACH(\
                UNS_RENUM_TOSTRING_SEGMENT,\
                UNS_RENUM_DELIMITER_NONE,\
                __VA_ARGS__\
            )\
            default: {\
                throw ::std::runtime_error{ "TODO" };\
            }\
        };\
    };\
    inline constexpr static renum_name from_string(const ::std::u8string_view& Str) {\
        UNS_RENUM_FOR_EACH(\
            UNS_RENUM_FROMSTRING_SEGMENT,\
            UNS_RENUM_DELIMITER_NONE,\
            __VA_ARGS__\
        )\
        {\
            throw ::std::runtime_error{ "TODO" };\
        };\
    };\
};\


namespace nmspc {

    UNS_RENUM(some, long int,
        (bcu, = -1229),
        (__, ),
        (abc, ),
        (engcef, )
    );

};

int main() {
    ::std::cout << "START\n";

    ::nmspc::some eval;

    auto strs = ::std::vector<::std::u8string>{};
    for (auto renum_val : ::nmspc::some::values()) {
        strs.emplace_back(renum_val.to_string());
    };

    ::std::cout << ::nmspc::some::size() << ::std::endl;

    ::std::cout << "FINISH\n";
};
