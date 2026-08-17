#!/usr/bin/env python3
"""
2-player Chinese Checkers: RED (top) vs ORANGE (bottom)
Fixed: cycle detection, better scoring, game actually finishes.
"""
import subprocess, time, sys, hashlib, os

# 共享工具：坐标换算 + 预编译 Swift 鼠标控制（click/move/activate/launch）
from jump_common import (board_to_screen, click as _click,
                         move_cursor as _move_cursor, launch_jump0)

def click(x, y):
    _click(x, y)
    time.sleep(0.45)

def move_cursor(x1, y1, x2, y2):
    _move_cursor(x1, y1, x2, y2)

# ── Piece colours ──────────────────────────────────────────────────────
def init_color(x, y):
    if x==5  and 1<=y<=4:    return 'RED'
    if x==6  and 2<=y<=4:    return 'RED'
    if x==7  and 3<=y<=4:    return 'RED'
    if x==8  and y==4:       return 'RED'
    if y==14 and 10<=x<=13:  return 'ORANGE'
    if y==15 and 11<=x<=13:  return 'ORANGE'
    if y==16 and 12<=x<=13:  return 'ORANGE'
    if y==17 and x==13:      return 'ORANGE'
    # Other colours (not played)
    if y==5  and 10<=x<=13:  return 'GREEN'
    if y==6  and 11<=x<=13:  return 'GREEN'
    if y==7  and 12<=x<=13:  return 'GREEN'
    if y==8  and x==13:      return 'GREEN'
    if x==14 and 10<=y<=13:  return 'ROSERED'
    if x==15 and 11<=y<=13:  return 'ROSERED'
    if x==16 and 12<=y<=13:  return 'ROSERED'
    if x==17 and y==13:      return 'ROSERED'
    if x==5  and 10<=y<=13:  return 'WHITE'
    if x==6  and 11<=y<=13:  return 'WHITE'
    if x==7  and 12<=y<=13:  return 'WHITE'
    if x==8  and y==13:      return 'WHITE'
    if y==5  and 1<=x<=4:    return 'BLUE'
    if y==6  and 2<=x<=4:    return 'BLUE'
    if y==7  and 3<=x<=4:    return 'BLUE'
    if y==8  and x==4:       return 'BLUE'
    return None

# Legal positions
LEGAL = set()
for _x in range(1,18):
    for _y in range(1,18):
        if init_color(_x,_y): LEGAL.add((_x,_y))
for _x in range(5,14):
    for _y in range(5,14): LEGAL.add((_x,_y))

def is_legal(x,y): return (x,y) in LEGAL

RED_TARGET    = {p for p in LEGAL if init_color(*p)=='ORANGE'}
ORANGE_TARGET = {p for p in LEGAL if init_color(*p)=='RED'}

# ── Board ──────────────────────────────────────────────────────────────
DIRS = [(0,-1),(-1,-1),(-1,0),(1,0),(0,1),(1,1)]

class Board:
    def __init__(self):
        self.grid = {(x,y):c for x in range(1,18) for y in range(1,18)
                     if (c:=init_color(x,y))}

    def occ(self,x,y):   return (x,y) in self.grid
    def color(self,x,y): return self.grid.get((x,y))

    def get_moves(self, x, y):
        result = set()
        for dx,dy in DIRS:
            nx,ny = x+dx,y+dy
            if is_legal(nx,ny) and not self.occ(nx,ny):
                result.add((nx,ny))
        # Jump BFS
        visited = {(x,y)}
        q = [(x,y)]
        while q:
            cx,cy = q.pop(0)
            for dx,dy in DIRS:
                nx,ny = cx+dx,cy+dy
                lx,ly = cx+2*dx,cy+2*dy
                if (self.occ(nx,ny) and is_legal(lx,ly)
                        and not self.occ(lx,ly) and (lx,ly) not in visited):
                    visited.add((lx,ly)); q.append((lx,ly))
                    result.add((lx,ly))
        return result

    def move(self, fx,fy, tx,ty):
        self.grid[(tx,ty)] = self.grid.pop((fx,fy))

    def pieces_of(self, color):
        return [(x,y) for (x,y),c in self.grid.items() if c==color]

    def state_key(self, color):
        pieces = sorted(self.pieces_of(color))
        return str(pieces)

# ── Improved strategy with cycle detection ────────────────────────────
def score_piece(x, y, color, target):
    """Score how well-placed this piece is (higher=better)."""
    if color == 'RED':
        base = y * 10  # advance toward bottom
    else:
        base = (18 - y) * 10  # advance toward top
    if (x,y) in target:
        base += 200  # big bonus for being in target
    return base

def board_score(board, color, target):
    return sum(score_piece(x,y,color,target) for x,y in board.pieces_of(color))

def choose_move(board, color, target, recent_states, forbidden_moves):
    """
    Pick the best move. Avoid moves that recreate recent board states
    or that are in the forbidden set (anti-cycle).
    """
    best_val, candidates = -99999, []

    pieces = board.pieces_of(color)
    cur_score = board_score(board, color, target)

    for px,py in pieces:
        for mx,my in board.get_moves(px,py):
            if (px,py,mx,my) in forbidden_moves:
                continue
            # Simulate move
            board.grid[(mx,my)] = board.grid.pop((px,py))
            new_key   = board.state_key(color)
            new_score = board_score(board, color, target)
            # Undo
            board.grid[(px,py)] = board.grid.pop((mx,my))

            # Penalise revisiting recent states
            repeat_penalty = recent_states.count(new_key) * 50

            gain = new_score - cur_score - repeat_penalty
            # Prefer jumps slightly (more progress per click)
            if abs(mx-px)+abs(my-py) > 1: gain += 3

            if gain > best_val:
                best_val = gain; candidates = [(px,py,mx,my)]
            elif gain == best_val:
                candidates.append((px,py,mx,my))

    if not candidates: return None
    # Break ties: pick piece furthest from target (helps unblock)
    if color == 'RED':
        candidates.sort(key=lambda m: m[1])           # lowest y first
    else:
        candidates.sort(key=lambda m: -m[1])          # highest y first
    return candidates[0]

def check_win(board, color, target):
    return all(p in target for p in board.pieces_of(color))

def do_move(board, color, mv, move_n):
    fx,fy,tx,ty = mv
    sx1,sy1 = board_to_screen(fx,fy)
    sx2,sy2 = board_to_screen(tx,ty)

    # Animate cursor then click
    move_cursor(sx1, sy1, sx1, sy1)   # move to source
    click(sx1, sy1)                    # select piece
    move_cursor(sx1, sy1, sx2, sy2)   # animate to dest
    click(sx2, sy2)                    # place piece

    board.move(fx,fy,tx,ty)
    print(f"  Move {move_n:3d} [{color:6s}] ({fx},{fy})→({tx},{ty})")

# ── Main ───────────────────────────────────────────────────────────────
def main():
    board = Board()

    # Ensure jump0 is running
    launch_jump0()

    print("""
╔══════════════════════════════════════════════════════╗
║   JUMPCHESS  RED 🔴 (top)  vs  ORANGE 🟠 (bottom)   ║
║   RED    → must fill ORANGE home (bottom triangle)  ║
║   ORANGE → must fill RED    home (top    triangle)  ║
╚══════════════════════════════════════════════════════╝""")

    # Focus window
    sx,sy = board_to_screen(9,9)
    move_cursor(sx,sy,sx,sy); click(sx,sy)
    time.sleep(0.3)

    players = [('RED',RED_TARGET),('ORANGE',ORANGE_TARGET)]
    move_n  = 0
    MAX     = 400
    recent  = {'RED':[], 'ORANGE':[]}   # last 8 state keys
    forbidden = set()                   # (fx,fy,tx,ty) blocked this turn

    for _ in range(MAX):
        color, target = players[move_n % 2]

        mv = choose_move(board, color, target, recent[color], forbidden)

        # If best move is blocked, lift ban and retry
        if mv is None:
            forbidden.clear()
            mv = choose_move(board, color, target, recent[color], set())
        if mv is None:
            print(f"\n{color} has no legal moves — STALEMATE"); break

        # Detect oscillation: same move as 2 turns ago → ban it
        fx,fy,tx,ty = mv
        if len(recent[color]) >= 2:
            prev_key = recent[color][-2]
            board.grid[(tx,ty)] = board.grid.pop((fx,fy))
            cur_key  = board.state_key(color)
            board.grid[(fx,fy)] = board.grid.pop((tx,ty))
            if cur_key == prev_key:
                forbidden.add((fx,fy,tx,ty))
                forbidden.add((tx,ty,fx,fy))   # also ban the reverse
                # Re-pick without these
                mv = choose_move(board, color, target, recent[color], forbidden)
                if mv is None:
                    forbidden.clear()
                    mv = choose_move(board, color, target, [], set())
                if mv is None:
                    print(f"\n{color} stuck — DRAW"); break
                fx,fy,tx,ty = mv

        do_move(board, color, mv, move_n+1)
        move_n += 1

        # Update recent state history (keep last 8)
        key = board.state_key(color)
        recent[color].append(key)
        if len(recent[color]) > 8: recent[color].pop(0)

        # Check win
        if check_win(board, color, target):
            print(f"""
╔══════════════════════════════════════════════════════╗
║  🏆  {color} WINS after {move_n} moves!
╚══════════════════════════════════════════════════════╝""")
            break

        time.sleep(0.1)
    else:
        print(f"\nMax moves ({MAX}) reached — DRAW")

if __name__ == '__main__':
    main()
