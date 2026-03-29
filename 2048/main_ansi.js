const util = require('util');
const addon = require('bindings')('g6-2048');
const prompt = require('prompt-sync')({ sigint: true });

GAME_2048_STATUS_NOT_OVER = 0;
GAME_2048_STATUS_WON = 1;
GAME_2048_STATUS_LOST = 2;

function dump(raw)
{
    reap = addon.peek(raw);
    result = '';
    for(let i = 0; i < reap.length; i++)
    {
        result += util.format('[%d, %d, %d, %d]%s',
            reap[i][0], reap[i][1], reap[i][2], reap[i][3],
            (i == (reap.length - 1))?'':'\n');
    }
    console.log(result);
}

function main()
{
    USAGE = '\
2048, a tile-based game.\n\
\tpress \'a\' or \'A\' for move-left,\n\
\tpress \'d\' or \'D\' for move-right,\n\
\tpress \'w\' or \'W\' for move-up,\n\
\tpress \'s\' or \'S\' for move-down,\n\
\tpress \'q\' or \'Q\' for quit.\n\
';
    console.log(USAGE);
    mat = addon.init();
    addon.add(mat);
    valid = true;
    kept = true;
    do {
        if(valid == true)
        {
            dump(mat);
        }
        const input = prompt("input (move): ");
        if(input.length > 0) {
            switch(input[0]) {
                case 'a':
                case 'A':
                    console.log('last move is %s', 'left');
                    mat = addon.left(mat);
                    st = addon.get(mat);
                    if(st == GAME_2048_STATUS_NOT_OVER) {
                        addon.add(mat);
                    }
                    else {
                        console.log('%s', (st == GAME_2048_STATUS_WON)?'Won':'Lost');
                        kept = false;
                    }
                    valid = true;
                    break;
                case 'd':
                case 'D':
                    console.log('last move is %s', 'right');
                    mat = addon.right(mat);
                    st = addon.get(mat);
                    if(st == GAME_2048_STATUS_NOT_OVER) {
                        addon.add(mat);
                    }
                    else {
                        console.log('%s', (st == GAME_2048_STATUS_WON)?'Won':'Lost');
                        kept = false;
                    }
                    valid = true;
                    break;
                case 'w':
                case 'W':
                    console.log('last move is %s', 'up');
                    mat = addon.up(mat);
                    st = addon.get(mat);
                    if(st == GAME_2048_STATUS_NOT_OVER) {
                        addon.add(mat);
                    }
                    else {
                        console.log('%s', (st == GAME_2048_STATUS_WON)?'Won':'Lost');
                        kept = false;
                    }
                    valid = true;
                    break;
                case 's':
                case 'S':
                    console.log('last move is %s', 'down');
                    mat = addon.down(mat);
                    st = addon.get(mat);
                    if(st == GAME_2048_STATUS_NOT_OVER) {
                        addon.add(mat);
                    }
                    else {
                        console.log('%s', (st == GAME_2048_STATUS_WON)?'Won':'Lost');
                        kept = false;
                    }
                    valid = true;
                    break;
                case 'q':
                case 'Q':
                    valid = true;
                    console.log('last move is %s', 'quit');
                    kept = false;
                    break;
                default:
                    valid = false;
                    console.log('Invalid input (%s)', input);
                    break;
            }
        }

        //kept = false;
    } while(kept == true);
}

main();

