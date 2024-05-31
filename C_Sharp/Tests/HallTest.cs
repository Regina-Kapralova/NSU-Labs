using FluentAssertions;
using Moq;
using NUnit.Framework;
using PickyBrideProblem;
using System;
using System.Collections.Generic;
using System.Text;

namespace Tests
{
    class HallTest
    {
        Hall hall;
        List<Contender> contenders;

        [SetUp]
        public void Setup()
        {
            Mock<IContenderGenerator> contenderGenerator = new Mock<IContenderGenerator>();
            contenders = CreateContenderList();
            contenderGenerator.Setup(contenderGenerator => contenderGenerator.InitContenderList()).Returns(contenders);
            hall = new Hall(contenderGenerator.Object);
            hall.Init();
        }

        List<Contender> CreateContenderList()
        {
            var contenders = new List<Contender>
            {
                new Contender("Артём", 1),
                new Contender("Виктор", 2),
                new Contender("Дмитрий", 3)
            };
            return contenders;
        }

        [Test]
        public void InviteContenderTest()
        {
            Contender firstContender = contenders[0];
            Contender secondContender = contenders[1];
            Contender thirdContender = contenders[2];
            hall.InviteContender().Should().BeEquivalentTo(firstContender); 
            hall.InviteContender().Should().BeEquivalentTo(secondContender); 
            hall.InviteContender().Should().BeEquivalentTo(thirdContender); 
        }

        [Test]
        public void ThrowExceptionWhenNoMoreContendersTest()
        {
            hall.InviteContender();
            hall.InviteContender();
            hall.InviteContender();
            hall.Invoking(f => f.InviteContender())
                .Should().Throw<Exception>().WithMessage("Error: no more contenders!");
        }

        [Test]
        public void IsInHallTest()
        {
            hall.IsInHall("Артём").Should().BeTrue();
            hall.InviteContender();
            hall.IsInHall("Артём").Should().BeFalse();
            hall.IsInHall("Виктор").Should().BeTrue();
            hall.InviteContender();
            hall.IsInHall("Виктор").Should().BeFalse();
        }

        [Test]
        public void GetMarkForFriendTest()
        {
            hall.InviteContender();
            hall.InviteContender();
            hall.InviteContender();
            hall.GetMarkForFriend("Артём").Should().Be(1);
            hall.GetMarkForFriend("Виктор").Should().Be(2);
            hall.GetMarkForFriend("Дмитрий").Should().Be(3);
        }

        [Test]
        public void GetMarkForPrincessTest()
        {
            hall.InviteContender();
            hall.InviteContender();
            hall.GetMarkForPrincess("Артём").Should().Be(1);
            hall.Invoking(f => f.GetMarkForPrincess("Виктор"))
                .Should().Throw<Exception>().WithMessage("Error: Princess is already married");
        }
    }
}
