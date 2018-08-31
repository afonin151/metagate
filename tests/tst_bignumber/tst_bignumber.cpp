#include "tst_bignumber.h"

#include "BigNumber.h"

tst_BigNumber::tst_BigNumber(QObject *parent)
    : QObject(parent)
{
}

void tst_BigNumber::testBigNumberDecimal_data()
{
    QTest::addColumn<QByteArray>("dec");
    QTest::newRow("BigNumberDecimal 01") << QByteArray("13874877844");
    QTest::newRow("BigNumberDecimal 02") << QByteArray("12787328744987349849839843893434894894398");
    QTest::newRow("BigNumberDecimal 03") << QByteArray("1");
    QTest::newRow("BigNumberDecimal 04") << QByteArray("12");
    QTest::newRow("BigNumberDecimal 05") << QByteArray("0");
    QTest::newRow("BigNumberDecimal 06") << QByteArray("-656565");
    QTest::newRow("BigNumberDecimal 07") << QByteArray("-1278732874498734984983984389343489489439812787328744987349849839843893434894894398");
    QTest::newRow("BigNumberDecimal 08") << QByteArray("90709905498549865896096590095409590690659096090484388954895896896589658968968968968965989070990549854986589609659009540959069065909609048438895489589689658965896896896896896598");
    QTest::newRow("BigNumberDecimal 09") << QByteArray("77777");
    QTest::newRow("BigNumberDecimal 10") << QByteArray("-1");
}

void tst_BigNumber::testBigNumberDecimal()
{
    QFETCH(QByteArray, dec);
    BigNumber num1(dec);
    QByteArray res1 = num1.getDecimal();
    QCOMPARE(dec, res1);

    BigNumber num2;
    num2.setDecimal(dec);
    QByteArray res2 = num2.getDecimal();
    QCOMPARE(dec, res2);

    BigNumber num3(num1);
    QByteArray res3 = num3.getDecimal();
    QCOMPARE(dec, res3);
}

void tst_BigNumber::testBigNumberSum_data()
{
    QTest::addColumn<QByteArray>("dec1");
    QTest::addColumn<QByteArray>("dec2");
    QTest::addColumn<QByteArray>("sum");
    QTest::newRow("BigNumberSum 01")
            << QByteArray("13874877844")
            << QByteArray("677878877866")
            << QByteArray("691753755710");
    QTest::newRow("BigNumberSum 02")
            << QByteArray("12787328744987349849839843893434894894398")
            << QByteArray("677878877866")
            << QByteArray("12787328744987349849839843894112773772264");
    QTest::newRow("BigNumberSum 03")
            << QByteArray("12787328744987349849839843893434894894398")
            << QByteArray("-12787328744987349849839843893434894894398")
            << QByteArray("0");
    QTest::newRow("BigNumberSum 04")
            << QByteArray("-12787328744987349849839843893434894894398")
            << QByteArray("12787328744987349849839843893434894894398")
            << QByteArray("0");
    QTest::newRow("BigNumberSum 05")
            << QByteArray("767567654545476766767567654545476766767567654545476766767567654545476766767567654545476766767567654545476766")
            << QByteArray("677878877866677878877866677878877866677878877866677878877866677878877866677878877866677878877866677878877866677878877866")
            << QByteArray("677878877867445446532412154645645434332424354633445446532412154645645434332424354633445446532412154645645434332424354632");
    QTest::newRow("BigNumberSum 06")
            << QByteArray("677878877867445446532412154645645434332424354633445446532412154645645434332424354633445446532412154645645434332424354632")
            << QByteArray("0")
            << QByteArray("677878877867445446532412154645645434332424354633445446532412154645645434332424354633445446532412154645645434332424354632");
    QTest::newRow("BigNumberSum 07")
            << QByteArray("0")
            << QByteArray("677878877867445446532412154645645434332424354633445446532412154645645434332424354633445446532412154645645434332424354632")
            << QByteArray("677878877867445446532412154645645434332424354633445446532412154645645434332424354633445446532412154645645434332424354632");
    QTest::newRow("BigNumberSum 08")
            << QByteArray("677878877867445446532412154645645434332424354633445446532412154645645434332424354633445446532412154645645434332424354632")
            << QByteArray("-4")
            << QByteArray("677878877867445446532412154645645434332424354633445446532412154645645434332424354633445446532412154645645434332424354628");
    QTest::newRow("BigNumberSum 09")
            << QByteArray("12787328744987349849839843893434894894398")
            << QByteArray("677878877866")
            << QByteArray("12787328744987349849839843894112773772264");
    QTest::newRow("BigNumberSum 10")
            << QByteArray("12787328744987349849839843893434894894398")
            << QByteArray("677878877866")
            << QByteArray("12787328744987349849839843894112773772264");
}

void tst_BigNumber::testBigNumberSum()
{
    QFETCH(QByteArray, dec1);
    QFETCH(QByteArray, dec2);
    QFETCH(QByteArray, sum);
    BigNumber num1(dec1);
    BigNumber num2;
    num2.setDecimal(dec2);
    BigNumber nums = num1 + num2;
    QByteArray res1 = nums.getDecimal();
    QCOMPARE(sum, res1);
    num1 += num2;
    QByteArray res2 = num1.getDecimal();
    QCOMPARE(sum, res2);
}

void tst_BigNumber::testBigNumberSub_data()
{
    QTest::addColumn<QByteArray>("dec1");
    QTest::addColumn<QByteArray>("dec2");
    QTest::addColumn<QByteArray>("sub");
    QTest::newRow("BigNumberSum 01")
            << QByteArray("677878877866")
            << QByteArray("123444334454")
            << QByteArray("554434543412");
    QTest::newRow("BigNumberSum 02")
            << QByteArray("12787328744987349849839843893434894894398")
            << QByteArray("12787328744987349849839843893434894894398")
            << QByteArray("0");
    QTest::newRow("BigNumberSum 03")
            << QByteArray("0")
            << QByteArray("-12787328744987349849839843893434894894398")
            << QByteArray("12787328744987349849839843893434894894398");
    QTest::newRow("BigNumberSum 04")
            << QByteArray("0")
            << QByteArray("12787328744987349849839843893434894894398")
            << QByteArray("-12787328744987349849839843893434894894398");
    QTest::newRow("BigNumberSum 05")
            << QByteArray("767567654545476766767567654545476766767567654545476766767567654545476766767567654545476766767567654545476766")
            << QByteArray("677878877866677878877866677878877866677878877866677878877866677878877866677878877866677878877866677878877866677878877866")
            << QByteArray("-677878877865910311223321201112110299023333401099910311223321201112110299023333401099910311223321201112110299023333401100");
    QTest::newRow("BigNumberSum 06")
            << QByteArray("677878877867445446532412154645645434332424354633445446532412154645645434332424354633445446532412154645645434332424354632")
            << QByteArray("0")
            << QByteArray("677878877867445446532412154645645434332424354633445446532412154645645434332424354633445446532412154645645434332424354632");
    QTest::newRow("BigNumberSum 07")
            << QByteArray("677878877867445446532412154645645434332424354633445446532412154645645434332424354633445446532412154645645434332424354632")
            << QByteArray("677878877867445446532412154645645434332424354633445446532412154645645434332424354633445446532412154645645434332424354632")
            << QByteArray("0");
    QTest::newRow("BigNumberSum 08")
            << QByteArray("677878877867445446532412154645645434332424354633445446532412154645645434332424354633445446532412154645645434332424354632")
            << QByteArray("-4")
            << QByteArray("677878877867445446532412154645645434332424354633445446532412154645645434332424354633445446532412154645645434332424354636");
    QTest::newRow("BigNumberSum 09")
            << QByteArray("-12787328744987349849839843893434894894398")
            << QByteArray("-677878877866")
            << QByteArray("-12787328744987349849839843892757016016532");
    QTest::newRow("BigNumberSum 10")
            << QByteArray("12787328744987349849839843893434894894398")
            << QByteArray("-677878877866")
            << QByteArray("12787328744987349849839843894112773772264");
}

void tst_BigNumber::testBigNumberSub()
{
    QFETCH(QByteArray, dec1);
    QFETCH(QByteArray, dec2);
    QFETCH(QByteArray, sub);
    BigNumber num1(dec1);
    BigNumber num2;
    num2.setDecimal(dec2);
    BigNumber nums = num1 - num2;
    QByteArray res1 = nums.getDecimal();
    QCOMPARE(sub, res1);
    num1 -= num2;
    QByteArray res2 = num1.getDecimal();
    QCOMPARE(sub, res2);
}

QTEST_MAIN(tst_BigNumber)
