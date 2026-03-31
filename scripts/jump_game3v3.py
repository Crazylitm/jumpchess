#!/usr/bin/env python3
"""
跳棋 3v3 高水平对决 v2 — 修复反向长跳 & 振荡问题
Team A: RED + GREEN + ROSERED  vs  Team B: ORANGE + WHITE + BLUE

AI 修复:
  - 去掉 dist*3 奖励（根本原因），跳棋仅+2小奖励
  - 方向惩罚：单颗棋子距目标距离增加则扣分
  - 逐棋位置历史：同一棋子原路返回直接禁止
  - 全局状态历史反振荡保留
"""

import subprocess, time

# ── Coordinate system ──────────────────────────────────────────────────
BEGX, BEGY    = 600, 30
XWIDTH, YHIGH = 30, 52
WIN_X, WIN_Y  = 0, 53

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
    return XCOL[x](y) if x in XCOL else 0

def board_to_screen(bx, by):
    px = BEGX + x_dis(bx,by)*XWIDTH
    py = BEGY + (by-1)*YHIGH
    return WIN_X+px, WIN_Y+py

def click(x, y):
    subprocess.run(['swift', '/tmp/clk.swift', str(x), str(y)], capture_output=True)
    time.sleep(0.38)

def move_cursor(x1, y1, x2, y2):
    script = f"""
import CoreGraphics, Foundation
let src = CGEventSource(stateID: .hidSystemState)
for i in 0...10 {{
    let t = Double(i)/10.0
    let pt = CGPoint(x: {x1}+Int(t*Double({x2}-{x1})),
                     y: {y1}+Int(t*Double({y2}-{y1})))
    let ev = CGEvent(mouseEventSource: src, mouseType: .mouseMoved,
                     mouseCursorPosition: pt, mouseButton: .left)
    ev?.post(tap: .cghidEventTap); usleep(18000)
}}
"""
    subprocess.run(['swift','-e',script], capture_output=True)

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

LEGAL = set()
for _x in range(1,18):
    for _y in range(1,18):
        if init_color(_x,_y): LEGAL.add((_x,_y))
for _x in range(5,14):
    for _y in range(5,14): LEGAL.add((_x,_y))

def is_legal(x,y): return (x,y) in LEGAL

TARGETS = {
    'RED':     frozenset(p for p in LEGAL if init_color(*p)=='ORANGE'),
    'ORANGE':  frozenset(p for p in LEGAL if init_color(*p)=='RED'),
    'GREEN':   frozenset(p for p in LEGAL if init_color(*p)=='WHITE'),
    'WHITE':   frozenset(p for p in LEGAL if init_color(*p)=='GREEN'),
    'ROSERED': frozenset(p for p in LEGAL if init_color(*p)=='BLUE'),
    'BLUE':    frozenset(p for p in LEGAL if init_color(*p)=='ROSERED'),
}

DIRS = [(0,-1),(-1,-1),(-1,0),(1,0),(0,1),(1,1)]

# ── Board ──────────────────────────────────────────────────────────────
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
        return tuple(sorted(self.pieces_of(color)))

# ── 精确六边形距离 ─────────────────────────────────────────────────────
def hex_dist(x1, y1, x2, y2):
    dx, dy = x2-x1, y2-y1
    if dx * dy >= 0:
        return max(abs(dx), abs(dy))
    else:
        return abs(dx) + abs(dy)

# 每颗棋子到最近目标格的距离
def piece_dist_to_target(px, py, target_set):
    if (px,py) in target_set:
        return 0
    return min(hex_dist(px,py,tx,ty) for tx,ty in target_set)

# ── 最优分配得分（贪心） ───────────────────────────────────────────────
def board_score(pieces, target):
    pieces_set = set(pieces)
    target_set = set(target)
    filled = len(pieces_set & target_set)

    pieces_out   = [p for p in pieces if p not in target_set]
    targets_free = [t for t in target_set if t not in pieces_set]

    if not pieces_out:
        return filled * 600

    total_dist = 0
    assigned = set()
    # 贪心：每颗棋选最近未被认领的目标
    for p in sorted(pieces_out, key=lambda q: min(
            hex_dist(q[0],q[1],t[0],t[1]) for t in targets_free) if targets_free else 0):
        best_d, best_t = 9999, None
        for t in targets_free:
            if t not in assigned:
                d = hex_dist(p[0],p[1],t[0],t[1])
                if d < best_d:
                    best_d, best_t = d, t
        if best_t:
            assigned.add(best_t)
            total_dist += best_d

    return filled * 600 - total_dist

# ── 高水平移动决策（修复版） ───────────────────────────────────────────
def choose_move(board, color, target, recent_states, piece_history, forbidden_moves):
    """
    piece_history: dict[piece_orig_pos → deque of recent positions]
    只记录"该棋子"上一个位置，移动后原路返回直接禁止
    """
    pieces = board.pieces_of(color)
    target_set = set(target)
    cur_score = board_score(pieces, target)

    best_val = -999999
    best_mv  = None

    for px,py in pieces:
        already_in = (px,py) in target_set
        prev_pos   = piece_history.get((px,py))   # 该棋上一个位置

        for mx,my in board.get_moves(px,py):
            # ── 硬性禁止 ──────────────────────────────────────
            if (px,py,mx,my) in forbidden_moves:
                continue
            # 逐棋反振荡：禁止原路返回
            if prev_pos == (mx,my):
                continue

            # ── 模拟移动，计算新分 ─────────────────────────────
            board.grid[(mx,my)] = board.grid.pop((px,py))
            new_key    = board.state_key(color)
            new_pieces = board.pieces_of(color)
            new_score  = board_score(new_pieces, target)
            board.grid[(px,py)] = board.grid.pop((mx,my))

            # 全局状态反振荡惩罚
            repeat_penalty = recent_states.count(new_key) * 120

            gain = new_score - cur_score - repeat_penalty

            # ── 方向性惩罚：单棋距目标变远则扣分 ────────────────
            if (px,py) not in target_set:
                old_d = piece_dist_to_target(px, py, target_set)
                new_d = piece_dist_to_target(mx, my, target_set)
                dir_delta = new_d - old_d   # 正值 = 变远
                if dir_delta > 0:
                    gain -= dir_delta * 15  # 每远1格扣15分

            # ── 保护已入目标的棋 ──────────────────────────────
            if already_in and (mx,my) not in target_set:
                gain -= 500

            # ── 进入目标区小奖励 ──────────────────────────────
            if (mx,my) in target_set and not already_in:
                gain += 30

            # ── 跳棋链：仅+2小奖励（打破平分时偏好跳） ──────────
            dist = abs(mx-px) + abs(my-py)
            if dist > 1:
                gain += 2

            if gain > best_val:
                best_val = gain
                best_mv  = (px,py,mx,my)

    return best_mv

def check_win(board, color, target):
    return all(p in target for p in board.pieces_of(color))

def do_move(board, color, mv, move_n, team, piece_history):
    fx,fy,tx,ty = mv
    sx1,sy1 = board_to_screen(fx,fy)
    sx2,sy2 = board_to_screen(tx,ty)
    move_cursor(sx1,sy1,sx1,sy1)
    click(sx1,sy1)
    move_cursor(sx1,sy1,sx2,sy2)
    click(sx2,sy2)
    board.move(fx,fy,tx,ty)

    # 更新逐棋历史：(fx,fy) 移到 (tx,ty)，记录 (tx,ty) 的上一步是 (fx,fy)
    piece_history[(tx,ty)] = (fx,fy)
    if (fx,fy) in piece_history:
        del piece_history[(fx,fy)]

    print(f"  #{move_n:4d} [Team{team}|{color:7s}] ({fx:2d},{fy:2d})→({tx:2d},{ty:2d})")

# ── Main ───────────────────────────────────────────────────────────────
def main():
    start_time = time.time()
    MAX_DURATION = 3600

    board = Board()

    r = subprocess.run(['pgrep','-x','jump0'], capture_output=True, text=True)
    if not r.stdout.strip():
        print("Launching jump0...")
        subprocess.Popen(['./jump0'], cwd='/Users/mike/claude-work/jumpchess/build',
                         stdout=subprocess.DEVNULL, stderr=subprocess.DEVNULL)
        time.sleep(4)
        subprocess.run(['swift', '/tmp/activate_jump0.swift'], capture_output=True)
        time.sleep(0.5)

    print("""
╔══════════════════════════════════════════════════════════════╗
║      🎯  JUMPCHESS 3v3 高水平专业对决 v2  🎯                ║
║                                                              ║
║  Team A 🔴🟢🌸  RED + GREEN + ROSERED                       ║
║  Team B 🟠⬜🔵  ORANGE + WHITE + BLUE                       ║
║                                                              ║
║  AI v2: 方向惩罚 + 逐棋反振荡 + 最优目标分配                ║
║  胜利: 己方3色全占对角区域 | 超1小时自动停止                 ║
╚══════════════════════════════════════════════════════════════╝
""")

    sx,sy = board_to_screen(9,9)
    move_cursor(sx,sy,sx,sy); click(sx,sy)
    time.sleep(0.3)

    TURN_ORDER = [
        ('RED',    'A', TARGETS['RED']),
        ('ORANGE', 'B', TARGETS['ORANGE']),
        ('GREEN',  'A', TARGETS['GREEN']),
        ('WHITE',  'B', TARGETS['WHITE']),
        ('ROSERED','A', TARGETS['ROSERED']),
        ('BLUE',   'B', TARGETS['BLUE']),
    ]
    TEAM_A = {'RED', 'GREEN', 'ROSERED'}
    TEAM_B = {'ORANGE', 'WHITE', 'BLUE'}

    color_done   = {c: False for c,_,_ in TURN_ORDER}
    recent       = {c: [] for c,_,_ in TURN_ORDER}   # 全局状态历史
    piece_hist   = {c: {} for c,_,_ in TURN_ORDER}   # 逐棋位置历史
    forbidden    = set()

    move_n   = 0
    MAX_MOVES = 4000
    stall_ctr = 0   # 连续跳过计数

    def team_score(team_set):
        return sum(len(set(board.pieces_of(c)) & set(TARGETS[c])) for c in team_set)

    for _ in range(MAX_MOVES):
        elapsed = time.time() - start_time
        if elapsed > MAX_DURATION:
            sa, sb = team_score(TEAM_A), team_score(TEAM_B)
            print(f"\n⏰ 运行超过1小时，自动停止。总手数: {move_n}")
            print(f"   Team A 到位: {sa}/30  |  Team B 到位: {sb}/30")
            if   sa > sb: print("🏆 Team A (RED+GREEN+ROSERED) 胜！")
            elif sb > sa: print("🏆 Team B (ORANGE+WHITE+BLUE) 胜！")
            else:         print("🤝 平局！")
            return

        color, team, target = TURN_ORDER[move_n % 6]
        if color_done[color]:
            move_n += 1
            continue

        mv = choose_move(board, color, target,
                         recent[color], piece_hist[color], forbidden)

        if mv is None:
            # 解除禁令重试
            forbidden.clear()
            mv = choose_move(board, color, target, recent[color], piece_hist[color], set())
        if mv is None:
            # 完全放松限制
            mv = choose_move(board, color, target, [], {}, set())
        if mv is None:
            print(f"  {color} 暂无合法移动，跳过")
            move_n += 1
            stall_ctr += 1
            if stall_ctr > 30:
                print("连续停滞过多，终止")
                break
            continue

        stall_ctr = 0

        # 全局反振荡检测
        fx,fy,tx,ty = mv
        if len(recent[color]) >= 2:
            board.grid[(tx,ty)] = board.grid.pop((fx,fy))
            cur_key = board.state_key(color)
            board.grid[(fx,fy)] = board.grid.pop((tx,ty))
            if cur_key == recent[color][-2]:
                forbidden.add((fx,fy,tx,ty))
                forbidden.add((tx,ty,fx,fy))
                mv = choose_move(board, color, target,
                                 recent[color], piece_hist[color], forbidden)
                if mv is None:
                    forbidden.clear()
                    mv = choose_move(board, color, target, [], {}, set())
                if mv is None:
                    move_n += 1
                    continue
                fx,fy,tx,ty = mv

        do_move(board, color, mv, move_n+1, team, piece_hist[color])
        move_n += 1

        key = board.state_key(color)
        recent[color].append(key)
        if len(recent[color]) > 14: recent[color].pop(0)

        if check_win(board, color, target):
            color_done[color] = True
            es = int(time.time()-start_time)
            print(f"\n  ✅ [{color}] (Team {team}) 全部到位！{es//60}分{es%60}秒")

        a_done = all(color_done[c] for c in TEAM_A)
        b_done = all(color_done[c] for c in TEAM_B)

        if a_done or b_done:
            es = int(time.time()-start_time)
            if a_done and b_done:
                print(f"\n🤝 DRAW！双方同时完成！总手数:{move_n} 用时:{es//60}分{es%60}秒")
            else:
                w = 'A 🔴🟢🌸' if a_done else 'B 🟠⬜🔵'
                print(f"""
╔══════════════════════════════════════════════════════════════╗
║  🏆  Team {w} 胜出！
║  总手数: {move_n}    用时: {es//60}分{es%60}秒
╚══════════════════════════════════════════════════════════════╝""")
            return

        time.sleep(0.05)

    sa, sb = team_score(TEAM_A), team_score(TEAM_B)
    print(f"\n达到最大手数({MAX_MOVES})。A:{sa}/30  B:{sb}/30")
    if sa>sb: print("🏆 Team A 胜！")
    elif sb>sa: print("🏆 Team B 胜！")
    else: print("🤝 平局")

if __name__ == '__main__':
    main()
