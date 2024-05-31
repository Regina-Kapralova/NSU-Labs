using FluentAssertions;
using Moq;
using NUnit.Framework;
using PickyBrideProblem;
using System;
using System.Collections.Generic;

namespace Tests
{
    class PrincessTest
    {
        private readonly int _amountOfContenders = 100;
        private Princess _princess;

        private void Init(List<Contender> contenders)
        {
            Mock<IContenderGenerator> contenderGenerator = new Mock<IContenderGenerator>();
            contenderGenerator.Setup(contenderGenerator => contenderGenerator.InitContenderList()).Returns(contenders);
            Hall hall = new Hall(contenderGenerator.Object);
            Friend friend = new Friend(hall);
            _princess = new Princess(hall, friend);
        }

        List<Contender> ContenderListForUnmarriedPrincess()
        {
            List<Contender> contendersForUnmarriedPrincess = new List<Contender>();
            for (int i = _amountOfContenders; i > 0; i--)
            {
                string name = "Contender_" + i;
                contendersForUnmarriedPrincess.Add(new Contender(name, i));
            }
            return contendersForUnmarriedPrincess;
        }

        [Test]
        public void PrincessUnmarried()
        {
            Init(ContenderListForUnmarriedPrincess());
            _princess.SelectBridegroom();
            const int LevelHappinessUnmarriedPrincess = 10;
            _princess.GetLevelHappiness().Should().Be(LevelHappinessUnmarriedPrincess);
        }

        List<Contender> ContenderListForPrincessMarriedContenderWithMark100()
        {
            List<Contender> contendersForPrincessMarriedContenderWithMark100 = new List<Contender>();
            for (int i = 1; i <= 0.3 * _amountOfContenders; i++)
            {
                string name = "Contender_" + i;
                contendersForPrincessMarriedContenderWithMark100.Add(new Contender(name, i));
            }
            contendersForPrincessMarriedContenderWithMark100.Add(new Contender("Contender_100", 100));
            for (int i = (int)0.3 * _amountOfContenders + 1; i <= _amountOfContenders - 1; i++)
            {
                string name = "Contender_" + i;
                contendersForPrincessMarriedContenderWithMark100.Add(new Contender(name, i));
            }
            return contendersForPrincessMarriedContenderWithMark100;
        }

        [Test]
        public void PrincessMarriedContenderWithMark100()
        {
            Init(ContenderListForPrincessMarriedContenderWithMark100());
            _princess.SelectBridegroom();
            const int LevelHappinessPrincessMarriedFirstContender = 20;
            _princess.GetLevelHappiness().Should().Be(LevelHappinessPrincessMarriedFirstContender);
        }

        List<Contender> ContenderListForMarriedBadContender()
        {
            List<Contender> contendersForUnhappyPrincess = new List<Contender>();
            for (int i = 1; i <= _amountOfContenders; i++)
            {
                string name = "Contender_" + i;
                contendersForUnhappyPrincess.Add(new Contender(name, i));
            }
            return contendersForUnhappyPrincess;
        }

        [Test]
        public void PrincessMarriedBadContender()
        {
            Init(ContenderListForMarriedBadContender());
            _princess.SelectBridegroom();
            const int PrincessIsUnhappy = 0;
            _princess.GetLevelHappiness().Should().Be(PrincessIsUnhappy);
        }

        List<Contender> ContenderListForThrowExceptionTest()
        {
            List<Contender> contendersForThrowExceptionTest = new List<Contender>();
            contendersForThrowExceptionTest.Add(new Contender("Contender", 1));
            return contendersForThrowExceptionTest;
        }

        [Test]
        public void ThrowExceptionWhenNoMoreContendersTest()
        {
            Init(ContenderListForThrowExceptionTest());
            _princess.Invoking(f => f.SelectBridegroom())
                  .Should().Throw<Exception>().WithMessage("Error: no more contenders!");
        }
    }
}
