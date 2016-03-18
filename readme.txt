使用方法:

1.先把mod的conversation.txt 和 已有的翻译文档dialogs.csv 拷贝到程序所在目录。
2.运行文件后可能会生成以下文件：
  dialogs_english.csv            所有对话的英文
  dialogs_unused.csv             dialogs里面已有的翻译，但该mod用不到
  dialogs_redundancy.csv         dialogs中的冗余翻译，mod不会显示出来的句子
  dialogs_todo.csv               这个文件就是你的工作文件，里面是尚未翻译的句子
  dialogs_todo_more.csv          游戏自带的Create Language Template隐藏的句子（如：作弊选项）
3.在dialogs_todo.csv中翻译好句子后，把鼠标指针移动到dialogs_todo.csv文件上，按住鼠标左键不要松开，把它拖动到程序上面后松开鼠标左键。程序将会生成最终的翻译文档：
  dialogs_done.csv
  该文件中包含dialogs中的有效翻译和dialogs_todo中的有效翻译 （不包含dialogs_unused.csv和dialogs_redundancy.csv中的翻译）
4.把dialogs_done.csv复制到mod的语言目录并重命名为dialogs.csv 工作彻底结束！

程序会自动隐藏不需要翻译的句子（如：全句就一个{s0}）
程序不会把dialogs_todo.csv中未改动的句子添加到dialogs_done.csv