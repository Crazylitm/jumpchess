#!/usr/bin/env python3
"""
2-player Chinese Checkers: RED (top) vs ORANGE (bottom)
Drives jump0 via CGEvent mouse clicks.
"""
import subprocess, time, sys

# ── Coordinate system ──────────────────────────────────────────────────
BEGX, BEGY   = 600, 30
XWIDTH, YHIGH = 30, 52
CHESS_RADIUS  = 19

# Window screen origin (from CGWindowList: X=0,Y=25 + title bar ~28px)
WIN_X, WIN_Y = 0, 53   # content area top-left in screen coords

SPECIAL_XDIS = {
    (13,5):-12,(13,6):-11,(12,5):-10,(13,7):-10,(12,6):-9,(13,8):-9,
    (11,5):-8, (12,7):-8, (13,9):-8, (11,6):-7, (12,8):-7,
    (10,5):-6, (11,7):-6,
    (1,5):12,(2,6):11,(2,5):10,(3,7):10,(3,6):9,(4,8):9,
    (3,5):8,(4,7):8,(5,9):8,(4,6):7,(5,8):7,(4,5):6,(5,7):6,
}
XCOL = {5:lambda y:y-1, 6:lambda y:y-3, 7:lambda y:y-5, 8:lambda y:y-7,
        9:lambda y:y-9, 10:lambda y:y-11,11:lambda y:y-13,12:lambda y:y-15,
        13:lambda y:y-17,14:lambda y:y-19,15:lambda y:y-21,
        16:lambda y:y-23,17:lambda y:y-25}

def x_dis(x, y):
    if (x,y) in SPECIAL_XDIS: return SPECIAL_XDIS[(x,y)]
    if x in XCOL: return XCOL[x](y)
    return 0

def board_to_screen(bx, by):
    px = BEGX + x_dis(bx, by) * XWIDTH
    py = BEGY + (by-1) * YHIGH
    return WIN_X + px, WIN_Y + py

def click(sx, sy, label=""):
    subprocess.run(['swift', '/tmp/clk.swift', str(sx), str(sy)],
                   capture_output=True)
    print(f"    🖱  click ({sx},{sy}) {label}")
    time.sleep(0.5)

# ── Piece colors ───────────────────────────────────────────────────────
def init_color(x, y):
    if x==5  and 1<=y<=4:    return 'RED'
    if x==6  and 2<=y<=4:    return 'RED'
    if x==7  and 3<=y<=4:    return 'RED'
    if x==8  and y==4:       return 'RED'
    if y==5  and 10<=x<=13:  return 'GREEN'
    if y==6  and 11<=x<=13:  return 'GREEN'
    if y==7  and 12<=x<=13:  return 'GREEN'
    if y==8  and x==13:      return 'GREEN'
    if x==14 and 10<=y<=13:  return 'ROSERED'
    if x==15 and 11<=y<=13:  return 'ROSERED'
    if x==16 and 12<=y<=13:  return 'ROSERED'
    if x==17 and y==13:      return 'ROSERED'
    if y==14 and 10<=x<=13:  return 'ORANGE'
    if y==15 and 11<=x<=13:  return 'ORANGE'
    if y==16 and 12<=x<=13:  return 'ORANGE'
    if y==17 and x==13:      return 'ORANGE'
    if x==5  and 10<=y<=13:  return 'WHITE'
    if x==6  and 11<=y<=13:  return 'WHITE'
    if x==7  and 12<=y<=13:  return 'WHITE'
    if x==8  and y==13:      return 'WHITE'
    if y==5  and 1<=x<=4:    return 'BLUE'
    if y==6  and 2<=x<=4:    return 'BLUE'
    if y==7  and 3<=x<=4:    return 'BLUE'
    if y==8  and x==4:       return 'BLUE'
    return None

# ── Legal positions (full board) ───────────────────────────────────────
LEGAL = set()
for _x in range(1, 18):
    for _y in range(1, 18):
        if init_color(_x,_y) is not None:
            LEGAL.add((_x,_y))
# Centre diamond (all cols 5-13, y range based on hex layout)
for _x in range(5, 14):
    for _y in range(5, 14):
        LEGAL.add((_x,_y))
# Extra legal cells connecting left/right wings
for _y in range(5,14):
    for _x in range(1,18):
        if init_color(_x,_y) is not None:
            LEGAL.add((_x,_y))

def is_legal(x, y):
    return (x,y) in LEGAL

# ── Target zones ───────────────────────────────────────────────────────
RED_TARGET    = {(x,y) for x in range(1,18) for y in range(1,18)
                 if init_color(x,y)=='ORANGE'}
ORANGE_TARGET = {(x,y) for x in range(1,18) for y in range(1,18)
                 if init_color(x,y)=='RED'}

# ── Board ──────────────────────────────────────────────────────────────
class Board:
    def __init__(self):
        self.grid = {}
        for x in range(1,18):
            for y in range(1,18):
                c = init_color(x,y)
                if c: self.grid[(x,y)] = c

    def occ(self, x, y):    return (x,y) in self.grid
    def color(self, x, y):  return self.grid.get((x,y))

    DIRS = [(0,-1),(-1,-1),(-1,0),(1,0),(0,1),(1,1)]

    def get_moves(self, x, y):
        result = set()
        # Single steps
        for dx,dy in self.DIRS:
            nx,ny = x+dx, y+dy
            if is_legal(nx,ny) and not self.occ(nx,ny):
                result.add((nx,ny))
        # Chain jumps (BFS)
        visited = {(x,y)}
        q = [(x,y)]
        while q:
            cx,cy = q.pop(0)
            for dx,dy in self.DIRS:
                nx,ny = cx+dx, cy+dy
                lx,ly = cx+2*dx, cy+2*dy
                if (self.occ(nx,ny) and is_legal(lx,ly)
                        and not self.occ(lx,ly) and (lx,ly) not in visited):
                    visited.add((lx,ly))
                    q.append((lx,ly))
                    result.add((lx,ly))
        return result

    def move(self, fx, fy, tx, ty):
        self.grid[(tx,ty)] = self.grid.pop((fx,fy))

    def pieces_of(self, color):
        return [(x,y) for (x,y),c in self.grid.items() if c==color]

# ── Strategy ───────────────────────────────────────────────────────────
def advance_score(x, y, color):
    """
    Higher = better progress toward goal.
    RED moves DOWN  (increasing y) → target y=14..17
    ORANGE moves UP (decreasing y) → target y=1..4
    """
    if color == 'RED':
        return y
    else:
        return -y

def choose_move(board, color, target):
    best_val, best_move = -9999, None
    pieces = board.pieces_of(color)
    for px,py in pieces:
        for mx,my in board.get_moves(px,py):
            gain  = advance_score(mx,my,color) - advance_score(px,py,color)
            bonus = 25 if (mx,my) in target else 0
            # Don't leave target unless forced
            penalty = -50 if (px,py) in target and (mx,my) not in target else 0
            val = gain + bonus + penalty
            # Prefer jumps over steps for same value
            dist = abs(mx-px)+abs(my-py)
            if dist > 1: val += 2  # slight jump preference
            if val > best_val:
                best_val, best_move = val, (px,py,mx,my)
    return best_move

def check_win(board, color, target):
    return all(p in target for p in board.pieces_of(color))

# ── ASCII board print ──────────────────────────────────────────────────
SYM = {'RED':'R','ORANGE':'O','GREEN':'G','ROSERED':'P',
       'WHITE':'W','BLUE':'B',None:'.'}

def print_board_state(board, move_n, color, frm, to):
    fx,fy = frm; tx,ty = to
    sx,sy = board_to_screen(fx,fy)
    dx,dy = board_to_screen(tx,ty)
    piece_sym = SYM[color]
    print(f"\n  Move {move_n:3d}  [{color:6s}]  "
          f"({fx},{fy})→({tx},{ty})   screen: ({sx},{sy})→({dx},{dy})")

# ── Main ───────────────────────────────────────────────────────────────
def main():
    board = Board()

    # Ensure jump0 is running
    r = subprocess.run(['pgrep','-x','jump0'], capture_output=True, text=True)
    if not r.stdout.strip():
        print("Launching jump0...")
        subprocess.Popen(['./jump0'], cwd='/Users/mike/claude-work/jumpchess/build',
                         stdout=subprocess.DEVNULL, stderr=subprocess.DEVNULL)
        time.sleep(4)

    print("""
╔══════════════════════════════════════════════════════╗
║   JUMPCHESS AUTO-PLAY: RED (top) vs ORANGE (bottom) ║
║   RED    → advancing DOWN  to ORANGE home zone      ║
║   ORANGE → advancing UP    to RED    home zone      ║
╚══════════════════════════════════════════════════════╝
  Window content origin: ({},{})
""".format(WIN_X, WIN_Y))

    # Click somewhere neutral first to focus window
    click(600, WIN_Y + 446, "focus window (centre)")
    time.sleep(0.5)

    players = [
        ('RED',    RED_TARGET),
        ('ORANGE', ORANGE_TARGET),
    ]
    move_n = 0
    MAX_MOVES = 300

    for _ in range(MAX_MOVES):
        color, target = players[move_n % 2]
        mv = choose_move(board, color, target)
        if not mv:
            print(f"\n{color} has no moves — DRAW / STALEMATE")
            break

        fx,fy,tx,ty = mv
        move_n += 1
        print_board_state(board, move_n, color, (fx,fy), (tx,ty))

        # Click piece then destination
        sx1,sy1 = board_to_screen(fx,fy)
        sx2,sy2 = board_to_screen(tx,ty)
        click(sx1, sy1, f"select {color} at ({fx},{fy})")
        click(sx2, sy2, f"move to ({tx},{ty})")

        board.move(fx, fy, tx, ty)

        if check_win(board, color, target):
            print(f"""
╔══════════════════════════════════════════════════════╗
║  🏆  {color} WINS after {move_n} moves!{' '*(27-len(color)-len(str(move_n)))}║
╚══════════════════════════════════════════════════════╝""")
            break

        # Short pause between turns so moves are visible on screen
        time.sleep(0.2)
    else:
        print(f"\nMax moves ({MAX_MOVES}) reached — DRAW")

if __name__ == '__main__':
    main()
