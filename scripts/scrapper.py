#!/usr/bin/env python

from random import sample
import bs4
import requests
import time
import traceback
import logging

def parseRequest(data):
    '''
    Parsing propokertools.com EV requests
    '''
    soup = bs4.BeautifulSoup(data, 'html.parser')
    table = soup.find('table', attrs={'class':'pptSimTable'})
    #print("FOUND TABLE --------------------->")
    #print(table)
    #print("END   TABLE --------------------->")
    rows = table.find_all('tr')

    p_data = []
    for row in rows:
        cols = row.find_all('td')
        cols = [ele.text.strip() for ele in cols]
        p_row = [ele for ele in cols if ele]
        if len(p_row) != 0:
            p_data.append(p_row)

    csv_line = r''
    for row in p_data:
        # [u'HAND', u'EV%', u'WINS', u'DRAWS']
        hand, ev = row[:2]
        hand_str = str(hand)
        ev_doubl = float(ev.strip('%'))
        csv_line += "{0},{1},".format(hand_str, ev_doubl)

    csv_line = csv_line.strip(',')
    print("[LOG]: '{0}'".format(csv_line))
    return csv_line

suits = 'hdsc'
figures = '23456789TJQKA'

deck = ['{}{}'.format(figure, suit) for suit in suits for figure in figures]

timeout = 1
tries   = 5
no_players = 8
c_players  = 2

HANDS = []
for try_ in range(tries):
    draw = sample(deck, c_players*no_players)
    hands = [draw[x:x+c_players] for x in range(0, len(draw), c_players)]
    hands_str = [''.join(hand) for hand in hands]
    HANDS.append(hands_str)

CSV_LINES = []
for hand in HANDS:
    hand_str = ["h{0}={1}".format(idx+1, hand_) for idx, hand_ in enumerate(hand)]
    hand_url = '&'.join(hand_str)
    url = r'http://www.propokertools.com/simulations/show?&g=he&{0}&s=generic'.format(hand_url)

    print("Sending request ... '{0}'".format(' vs '.join(hand)))
    print("[URL]: {0}".format(url))
    try:
        r = requests.get(url, timeout=5)
        data = r.text
        csv_line = parseRequest(data)
    except Exception as e:
        logging.error(traceback.format_exc())
        csv_line = r''

    if csv_line:
        CSV_LINES.append(csv_line)

    time.sleep(timeout)

with open('{0}hands_new.csv'.format(no_players), 'a') as file_:
    if CSV_LINES:
        file_.write('\n'.join(CSV_LINES))
        file_.write('\n')

