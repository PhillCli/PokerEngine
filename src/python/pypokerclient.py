#!/usr/bin/env python
from pypoker import PokerEngine # Boost Python C++ wrapper
import time

# data from http://www.propokertools.com/simulations
data_2hands = {
    "AhKh AcKc": [50.00, 50.00],
    "2s3s AcKc": [36.27, 63.73],
    "2s4s 5h9s": [37.63, 62.37],
    "AsQs JcJd": [46.04, 53.96],
    "4c7h 4s5h": [64.58, 35.42],
    "6cTc 3cAc": [40.58, 59.42],
    "3d6c 8cAh": [34.61, 65.39],
    "5h2c Jh4c": [37.44, 62.56],
    "5s4c Th2s": [45.05, 54.95],
    "Js6s AcTs": [38.79, 61.21]
}

data_Xhands = {
    "AhKh AcKc 2s3s": [31.06, 31.06, 37.87],
    "AhKh AcKc 2s3s 5s4c": [25.18, 23.18, 22.28, 29.35],
    "AhKh AcKc 2s3s 5s4c QcQh": [17.62, 15.78, 14.71, 14.23, 37.66],
}

# general test case
def test_xhand_ev(type_='fast', data_set=data_2hands):
    engine = PokerEngine()
    if type_=='fast':
        runs = 1e5
        test_acc = 5e-1
    else:
        runs = 1e6
        test_acc = 1.6e-1 # (5e-1)/(10**0.5) = 1.58e-1 # statistics

    header_str = r"[ P1, P2, ..., [ EV1, EV2, ..., [ RUNS, [ SPEED, [ STATUS ]]]]]"
    sep = '%s' % ('-'*len(header_str))
    print(sep)
    print(header_str)

    for case in data_set.keys():
        hands = list(case.split())

        # C++ API call
        time_start = time.time()
        result = engine.EV(hands, "", runs)
        time_spent = time.time() - time_start

        # results = [EV1, EV2, ..., EVN, NO_RUNS]
        evs = map(lambda x: 100*x, result[:-1])
        runs = result[-1]

        small_spacing = '  '
        coll_spacing = ' | '
        hands_str = small_spacing.join(hands)
        ev_str = small_spacing.join(['%.2f'%(ev) for ev in evs])

        test_value = lambda val, ref, thresh=1e-1: abs(val-ref) < thresh
        assert_ = [test_value(ev, ev_ref, test_acc) for ev, ev_ref in zip(evs, data_set[case])]
        if all(assert_):
            status = "ok!"
        else:
            status = "failed!"

        # log formmating
        run_per_sec = runs/time_spent
        result_line = hands_str + coll_spacing + ev_str + coll_spacing + '%.2e'%runs + coll_spacing + "%.3e"%run_per_sec  + coll_spacing + status
        sep = '%s' % ('-'*len(result_line))
        print(sep)
        print(result_line)


def EV(hands, board='', runs='1e6'):
    """
    Return Hands EV

    >>> '%.1f %.1f' % EV(["AhKh", "AcKc"], runs=2e6)
    '50.0 50.0'

    >>> '%.f %.f' % EV(["2s3s", "AcKc"], runs=5e5)
    '36 64'

    >>> '%.f %.f' % EV(["2s4s", "5h9s"], runs=5e5)
    '38 62'
    """
    engine = PokerEngine()
    result = engine.EV(hands, board, runs)
    p1, p2, runs = result
    p1, p2 = (100*p1, 100*p2)
    return (p1, p2)

def main():
    print("[TEST] Testing Python bindings ...")
    test_xhand_ev('fast', data_set = data_2hands)
    test_xhand_ev('fast', data_set = data_Xhands)

if __name__ == '__main__':
    #import doctest
    #doctest.testmod()
    main()
