# LCD-like-Snake-game
A simple snake game with a LCD like display on windows system. (C++ Win32 API)

A simple Tetris game with a LCD like display on windows system. C++ with Win32 API

这是我最喜欢的小游戏之一，我比较推崇那种LCD点阵式的那种复古显示，比较经典。简单解释下我写这个程序的思路和代码组织结构。

1.这个程序使用的Win32的一些函数，C++写的

2.关于UI的核心，我是用的是Win32的基础绘图函数，根据游戏的需要重新包装进类GraphicsManager.h里。这个类完成所有必要的绘制工作。

3.有一个snake类，该类定义了蛇的数据结构和一些逻辑功能。 4.gameControl.h类用来，对游戏进行控制。绝大多数的控制都在这里完成的。

4.snakegame类，是winmain函数所在的位置，主要负责和操作系统交互，以及对一些按键的响应。

5.有一个可以更新还未完成的地方就是：当用户在一个时间处理周期内如果输入了2个或2个以上转向指令，系统将只接受最后一次指令，之前的指令都被忽略。这在一些时候并不特别理想，最好的是能在一个时间处理周期内接受2个指令。

6.在写贪食蛇游戏的时候，如何处理并合理显示可能存在的平行的蛇体段时的视觉效果，是需要话费一些心思的。 

7.代码没有优化，仅供参考。


