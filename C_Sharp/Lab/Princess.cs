using Microsoft.Extensions.Hosting;
using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading;
using System.Threading.Tasks;

namespace PickyBrideProblem
{
    /// <summary>
    /// Princess wants to get married successfully.
    /// </summary>
    public class Princess : IHostedService
    {
        private IHallForPrincess _hall;
        private Friend _friend;
        //  sorted list of contenders, who was not chosen by princess 
        private readonly List<IContender> _exContenders;
        private int _amountOfExContenders;
        private int _levelHappinessPrincess;
        private IHostApplicationLifetime _lifeTime;

        public Princess(IHallForPrincess hall, Friend friend)
        {
            _hall = hall;
            _friend = friend;
            _exContenders = new List<IContender>();
            _amountOfExContenders = 0;
        }

        /// <summary>
        /// Princess remembers what place the missed Contender has in her own ranking(_exContenders).
        /// </summary>
        private void SkipContender(IContender contender)
        {
            if (contender == null) return;
            int start = 0, end = _exContenders.Count;
            // if current contender the worst, put it in _exContenders[0]
            if (end > 0 && _friend.Compare(contender, _exContenders[0]) != contender)
            {
                _exContenders.Insert(0, contender);
                _amountOfExContenders++;
                return;
            }
            // using binary search, find position for current contender in sortered list
            int center = (end - start) / 2;
            while (end - start > 1)
            {
                if (_friend.Compare(contender, _exContenders[center]) == contender)
                {
                    start = center;
                    center = (end - start) / 2 + start;
                }
                else
                {
                    end = center;
                    center = (end - start) / 2;
                }
            }
            // insert current contender in list on finded position
            _exContenders.Insert(end, contender);
            _amountOfExContenders++;
        }

        /// <summary>
        /// Princess select Contender, who will become her husband.
        /// </summary>
        public void SelectBridegroom()
        {
            _hall.Init();
            IContender contender;
            // Princess skiped 30% contenders
            const double partSkipedContenders = 0.3;
            for (int i = 1; i <= partSkipedContenders * _hall.AmountOfContenders; i++)
            {
                contender = _hall.InviteContender();
                SkipContender(contender);
            }
            // Then princess find contender that better than _exContenders[_exContenders.Count - 2]
            while (_amountOfExContenders < _hall.AmountOfContenders)
            {
                contender = _hall.InviteContender();
                if (_friend.Compare(contender, _exContenders[_exContenders.Count - 2]) == contender)
                {
                    // if finded, get married
                    ChooseContender(contender);
                    return;
                }
                SkipContender(contender);
            }
            // if didn't find, don't get married
            ChooseContender(null);
            return;
        }

        /// <summary>
        /// Princess count her lavel happiness.
        /// </summary>
        private void ChooseContender(IContender contender)
        {
            const int PrincessIsUnhappy = 0;
            const int LevelHappinessUnmarriedPrincess = 10;
            const int LevelHappinessPrincessMarriedFirstContender = 20;
            const int LevelHappinessPrincessMarriedThirdContender = 50;
            const int LevelHappinessPrincessMarriedFifthContender = 100;
            if (contender == null)
            {
                _levelHappinessPrincess = LevelHappinessUnmarriedPrincess;
                return;
            }
            int mark = _hall.GetMarkForPrincess(contender.Name);
            if (mark == 100)
            {
                _levelHappinessPrincess = LevelHappinessPrincessMarriedFirstContender;
            }
            else if (mark == 98)
            {
                _levelHappinessPrincess = LevelHappinessPrincessMarriedThirdContender;
            }
            else if (mark == 96)
            {
                _levelHappinessPrincess = LevelHappinessPrincessMarriedFifthContender;
            }
            else
            {
                _levelHappinessPrincess = PrincessIsUnhappy;
            }
        }

        public int GetLevelHappiness()
        {
            return _levelHappinessPrincess;
        }

        public Task StartAsync(CancellationToken cancellationToken)
        {
            try
            {
                SelectBridegroom();
                Console.WriteLine(_levelHappinessPrincess);
            }
            catch (Exception ex)
            {
                Console.WriteLine(ex.Message);
            }
            finally
            {
                _lifeTime.StopApplication();
            }
            return Task.CompletedTask;
        }

        public Task StopAsync(CancellationToken cancellationToken)
        {
            return Task.CompletedTask;
        }
    }
}
