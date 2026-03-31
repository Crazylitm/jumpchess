#!/usr/bin/env python3
"""
Jumpchess 2-player simulator: RED (top) vs ORANGE (bottom)
Calculates pixel coords, sends mouse clicks to jump0 window.
"""
import subprocess, time, math, sys

# ── Board constants (mirrors CheckersUI.h / CheckersMapLimitCheck.cpp) ──
BEGX   = 600   # RED_TOP_5_1_X = WINDOW_WIDTH/2
BEGY   = 30    # RED_TOP_5_1_Y
XWIDTH = 30
YHIGH  = 52
CHESS_RADIUS = 19

# ── Hardcoded x_dis lookup (from getDistanceFromRED_5_1_point_X) ──────
SPECIAL = {
    (13,5):-12,(13,6):-11,(12,5):-10,(13,7):-10,(12,6):-9,(13,8):-9,
    (11,5):-8, (12,7):-8, (13,9):-8, (11,6):-7, (12,8):-7, (10,5):-6,
    (11,7):-6,
    (1,5):12,(2,6):11,(2,5):10,(3,7):10,(3,6):9,(4,8):9,
    (3,5):8,(4,7):8,(5,9):8,(4,6):7,(5,8):7,(4,5):6,(5,7):6,
}
XCOL_FORMULA = {
    5: lambda y: y-1,
    6: lambda y: (y-2)-1,
    7: lambda y: (y-3)-2,
    8: lambda y: (y-4)-3,
    9: lambda y: (y-5)-4,
    10:lambda y: (y-6)-5,
    11:lambda y: (y-7)-6,
    12:lambda y: (y-8)-7,
    13:lambda y: (y-9)-8,
    14:lambda y: (y-10)-9,
    15:lambda y: (y-11)-10,
    16:lambda y: (y-12)-11,
    17:lambda y: (y-13)-12,
}

def x_dis(x, y):
    if (x,y) in SPECIAL: return SPECIAL[(x,y)]
    if x in XCOL_FORMULA: return XCOL_FORMULA[x](y)
    return 0

def board_to_pixel(x, y):
    px = BEGX + x_dis(x,y) * XWIDTH
    py = BEGY + (y-1) * YHIGH
    return (px, py)

# ── Colour definitions ──
def get_color(x, y):
    if x==5 and 1<=y<=4: return 'RED'
    if x==6 and 2<=y<=4: return 'RED'
    if x==7 and 3<=y<=4: return 'RED'
    if x==8 and y==4:    return 'RED'
    if y==5  and 10<=x<=13: return 'GREEN'
    if y==6  and 11<=x<=13: return 'GREEN'
    if y==7  and 12<=x<=13: return 'GREEN'
    if y==8  and x==13:     return 'GREEN'
    if x==14 and 10<=y<=13: return 'ROSERED'
    if x==15 and 11<=y<=13: return 'ROSERED'
    if x==16 and 12<=y<=13: return 'ROSERED'
    if x==17 and y==13:     return 'ROSERED'
    if y==14 and 10<=x<=13: return 'ORANGE'
    if y==15 and 11<=x<=13: return 'ORANGE'
    if y==16 and 12<=x<=13: return 'ORANGE'
    if y==17 and x==13:     return 'ORANGE'
    if x==5 and 10<=y<=13: return 'WHITE'
    if x==6 and 11<=y<=13: return 'WHITE'
    if x==7 and 12<=y<=13: return 'WHITE'
    if x==8 and y==13:     return 'WHITE'
    if y==5  and 1<=x<=4:  return 'BLUE'
    if y==6  and 2<=x<=4:  return 'BLUE'
    if y==7  and 3<=x<=4:  return 'BLUE'
    if y==8  and x==4:     return 'BLUE'
    return 'EMPTY'

# ── Legal position (simplified) ──
MAP_RANGE = {
    1: (5,5), 2:(5,6), 3:(5,7), 4:(5,8), 5:(5,13),
    6:(5,13), 7:(5,13), 8:(5,13), 9:(5,13),
    10:(5,14), 11:(5,14), 12:(5,14), 13:(5,17),
    14:(10,13), 15:(11,13), 16:(12,13), 17:(13,13),
}
# Build legal set from color map
LEGAL = set()
for cx in range(1,18):
    for cy in range(1,18):
        c = get_color(cx,cy)
        if c != 'EMPTY': LEGAL.add((cx,cy))
# Add centre
for cx in range(5,14):
    for cy in range(5,14):
        if 5<=cx<=13 and 5<=cy<=13:
            LEGAL.add((cx,cy))
# Full legal set from the C++ mapXCountToYMin2Max — rebuild manually
LEGAL.clear()
ranges = {
    5:(1,4), 6:(2,4), 7:(3,4), 8:(4,4),
    9:(5,5),
    10:(5,6), 11:(5,7), 12:(5,8), 13:(5,17),
    14:(6,13), 15:(7,13), 16:(8,13), 17:(9,13),  # approximate
}
# Simpler: just enumerate all positions that are colored or in the centre diamond
# Read from the startup grid: 17 columns, rows 1..17
# Centre region where all x from 5..13 have valid y positions:
for x in range(1,18):
    for y in range(1,18):
        c = get_color(x,y)
        if c != 'EMPTY':
            LEGAL.add((x,y))

# Add the centre/path positions (SHARELIGHTGREEN = playable empty)
# From the grid output: centre row 5 (y=5) goes x=5..13
# We'll trust the colour function + add all grid positions
# Use IsLegalPosition approximation
def is_legal(x,y):
    if x<1 or x>17 or y<1 or y>17: return False
    c = get_color(x,y)
    if c != 'EMPTY': return True
    # Centre area
    if 5<=x<=13 and 5<=y<=13: return True
    return False

# ── Board state ──
class Board:
    def __init__(self):
        self.pieces = {}  # (x,y) -> color
        for x in range(1,18):
            for y in range(1,18):
                c = get_color(x,y)
                if c != 'EMPTY':
                    self.pieces[(x,y)] = c

    def occupied(self, x, y):
        return (x,y) in self.pieces

    def get_neighbors(self, x, y):
        """6 directions on hex grid"""
        return [
            (x,   y-1), (x-1, y-1),   # top-right, top-left
            (x-1, y  ), (x+1, y  ),   # mid-left,  mid-right
            (x,   y+1), (x+1, y+1),   # bot-left,  bot-right
        ]

    def get_moves(self, x, y):
        """Single steps + chain jumps"""
        moves = []
        for nx, ny in self.get_neighbors(x, y):
            if is_legal(nx,ny) and not self.occupied(nx, ny):
                moves.append((nx,ny))
        # Jumps (BFS)
        visited = {(x,y)}
        queue = [(x,y)]
        while queue:
            cx,cy = queue.pop(0)
            for nx,ny in self.get_neighbors(cx,cy):
                if self.occupied(nx,ny):
                    # landing spot
                    lx = nx + (nx-cx)
                    ly = ny + (ny-cy)
                    if is_legal(lx,ly) and not self.occupied(lx,ly) and (lx,ly) not in visited:
                        visited.add((lx,ly))
                        queue.append((lx,ly))
                        moves.append((lx,ly))
        return list(set(moves))

    def move(self, fx, fy, tx, ty):
        color = self.pieces.pop((fx,fy))
        self.pieces[(tx,ty)] = color

# ── Target zones: RED→ORANGE zone, ORANGE→RED zone ──
RED_TARGET = {(x,y) for x in range(10,14) for y in range(14,18) if get_color(x,y)=='ORANGE'}
RED_TARGET |= {(12,15),(13,15),(11,15),(13,16),(12,16),(13,17)}
# Rebuild properly:
RED_TARGET = set()
for x in range(1,18):
    for y in range(1,18):
        if get_color(x,y) == 'ORANGE': RED_TARGET.add((x,y))

ORANGE_TARGET = set()
for x in range(1,18):
    for y in range(1,18):
        if get_color(x,y) == 'RED': ORANGE_TARGET.add((x,y))

def red_score(x,y):
    """How far RED piece has advanced toward ORANGE zone (higher=better)"""
    return y  # y increases toward bottom (ORANGE zone)

def orange_score(x,y):
    """How far ORANGE piece has advanced toward RED zone (lower y=better)"""
    return -y  # y decreases toward top (RED zone)

def best_move(board, color, score_fn, target):
    """Greedy: pick the move that maximises total score for all pieces of color."""
    best = None
    best_val = -9999
    pieces = [(x,y) for (x,y),c in board.pieces.items() if c==color]
    for px,py in pieces:
        moves = board.get_moves(px, py)
        for mx,my in moves:
            # score gain for this piece
            gain = score_fn(mx,my) - score_fn(px,py)
            # bonus for landing in target zone
            bonus = 20 if (mx,my) in target else 0
            # penalty for leaving target zone if already inside
            penalty = -30 if (px,py) in target and (mx,my) not in target else 0
            val = gain + bonus + penalty
            if val > best_val:
                best_val = val
                best = (px,py,mx,my)
    return best

# ── Mouse click via osascript ──
def click(wx, wy, px, py):
    """Click screen position (wx+px, wy+py) using osascript"""
    sx = wx + px
    sy = wy + py
    script = f'''
tell application "System Events"
    tell process "jump0"
        set frontmost to true
    end tell
end tell
do shell script "cliclick c:{sx},{sy}"
'''
    # Try osascript mouse event
    scr2 = f'''
tell application "System Events"
    set frontmost of process "jump0" to true
    click at {{{sx},{sy}}}
end tell
'''
    result = subprocess.run(['osascript','-e',scr2], capture_output=True, text=True)
    if result.returncode != 0:
        print(f"  [click err] {result.stderr.strip()[:80]}")
    else:
        print(f"  ✓ clicked ({sx},{sy})")
    time.sleep(0.4)

def get_window_pos():
    """Try to get jump0 window position"""
    script = '''
tell application "System Events"
    tell process "jump0"
        try
            set w to first window
            set b to bounds of w
            return b
        on error
            return "0,0,1200,900"
        end try
    end tell
end tell
'''
    r = subprocess.run(['osascript','-e',script], capture_output=True, text=True)
    txt = r.stdout.strip()
    print(f"Window bounds: {txt}")
    try:
        parts = [int(p.strip()) for p in txt.split(',')]
        return parts[0], parts[1]
    except:
        return 0, 0  # fallback: assume top-left at 0,0

def print_board(board, move_num, player, frm, to):
    fx,fy = frm; tx,ty = to
    fpx,fpy = board_to_pixel(fx,fy)
    tpx,tpy = board_to_pixel(tx,ty)
    print(f"\n── Move {move_num:2d} [{player}] ({fx},{fy})→({tx},{ty})  "
          f"pixels: ({fpx},{fpy})→({tpx},{tpy})")

def check_win(board, color, target):
    pieces = [(x,y) for (x,y),c in board.pieces.items() if c==color]
    return all(p in target for p in pieces)

# ── Main simulation ──
def main():
    board = Board()
    wx, wy = get_window_pos()
    print(f"Window origin: ({wx},{wy})")

    # Launch jump0 if not running
    r = subprocess.run(['pgrep','-x','jump0'], capture_output=True, text=True)
    if not r.stdout.strip():
        print("Launching jump0...")
        subprocess.Popen(['./jump0'], cwd='/Users/mike/claude-work/jumpchess/build',
                         stdout=subprocess.DEVNULL, stderr=subprocess.DEVNULL)
        time.sleep(3)
        wx, wy = get_window_pos()

    print(f"\n{'='*60}")
    print("  JUMPCHESS SIMULATION: RED (top) vs ORANGE (bottom)")
    print(f"  RED target:    ORANGE zone  (y=14..17)")
    print(f"  ORANGE target: RED zone     (y=1..4)")
    print(f"{'='*60}")

    max_moves = 200
    move_num = 0
    players = [('RED', red_score, RED_TARGET), ('ORANGE', orange_score, ORANGE_TARGET)]
    turn = 0

    for _ in range(max_moves):
        color, score_fn, target = players[turn % 2]
        mv = best_move(board, color, score_fn, target)
        if mv is None:
            print(f"\n{color} has no moves — STALEMATE")
            break

        fx,fy,tx,ty = mv
        move_num += 1
        print_board(board, move_num, color, (fx,fy), (tx,ty))

        # Send mouse clicks to jump0 window
        fpx, fpy = board_to_pixel(fx, fy)
        tpx, tpy = board_to_pixel(tx, ty)
        click(wx, wy, fpx, fpy)   # select piece
        time.sleep(0.3)
        click(wx, wy, tpx, tpy)   # move to target

        board.move(fx, fy, tx, ty)

        # Check win
        if check_win(board, color, target):
            print(f"\n{'='*60}")
            print(f"  🏆  {color} WINS after {move_num} moves!")
            print(f"{'='*60}")
            break

        turn += 1
    else:
        print(f"\n Draw / max moves reached ({max_moves})")

if __name__ == '__main__':
    main()
