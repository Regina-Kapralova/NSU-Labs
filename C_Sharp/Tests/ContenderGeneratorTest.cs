using FluentAssertions;
using NUnit.Framework;
using PickyBrideProblem;
using System;
using System.Collections.Generic;
using System.Linq;

namespace Tests
{
    public class ContenderGeneratorTest
    {
        private List<Contender> _contenderList;
        private int _amountOfContenders;
        private ContenderGenerator _contenderGenerator;

        public ContenderGeneratorTest()
        {
            _contenderGenerator = new ContenderGenerator();
            _amountOfContenders = 100;
            _contenderGenerator.Init(_amountOfContenders);
            _contenderList = _contenderGenerator.InitContenderList();
        }

        [Test]
        public void UniqueNamesTest()
        {
            var contenderNames = _contenderList.Select(contender => contender.Name);
            contenderNames.Should().OnlyHaveUniqueItems();
        }

        [Test]
        public void UniqueMarksTest()
        {
            var contenderMarks = _contenderList.Select(contender => contender.Mark);
            contenderMarks.Should().OnlyHaveUniqueItems();
        }

        [Test]
        public void CreateContendersListOf100ContendersTest()
        {
            _contenderList.Count.Should().Be(_amountOfContenders);
        }

        [TestCase(-1)]
        [TestCase(-5)]
        public void ThrowExceptionCreateListOfNegativeAmountContendersTest(int amountOfContenders)
        {
            ContenderGenerator newContenderGenerator = new ContenderGenerator();
            newContenderGenerator.Invoking(f => f.Init(amountOfContenders))
                .Should().Throw<Exception>().WithMessage("Error: impossible to create list of less than 0 or more than 100 Contenders!");
        }

        [TestCase(101)]
        [TestCase(150)]
        public void ThrowExceptionCreateListOfAmountMore100ContendersTest(int amountOfContenders)
        {
            ContenderGenerator newContenderGenerator = new ContenderGenerator();
            newContenderGenerator.Invoking(f => f.Init(amountOfContenders))
                .Should().Throw<Exception>().WithMessage("Error: impossible to create list of less than 0 or more than 100 Contenders!");
        }
    }
}