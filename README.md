## README

### 开发环境

* Linux: Ubuntu18.04
* g++: 7.5.0
* jsoncpp: 0.5.0
* redis: 6.0.8
* hiredis: 1.0.0
* redis-plus-plus
* cppjieba

### 推荐编码规范

* [Google 开源项目风格指南 (中文版)](https://github.com/zh-google-styleguide/zh-google-styleguide)

### VSCode设置

* [Ubuntu下使用VSCode连接Github](http://bitjoy.net/2018/11/13/ubuntu%E4%B8%8B%E4%BD%BF%E7%94%A8vscode%E8%BF%9E%E6%8E%A5github/)

### 项目注意事项

* 中英文版本分开编写，中文版本后缀 `_zh` / `ZH`，英文版本后缀 `_en` / `EN`
* 请勿直接改写本地Git库，应将内容复制到其他文件夹下进行改写，完成改写并进行测试后再写回本地库(注意复制的时候不要复制隐藏文件夹 `.git`)
* 为避免冲突，上传更改前先与云端同步，拉取最新版本后，再提交本次修改并推送（`Fetch` - `Pull` - `Commit` - `Push`）
* 命名空间与项目名称一致（`keyword_suggestion` / `web_query`）

### 使用说明

* 使用项目功能前请先在配置文件中添加信息。例如，本机用户名为 `usr`，IP地址为 `ip`，端口号为 `port`（端口号保证不冲突和一致性即可），则需要在c / s端的 `net.conf` 中添加以下两行信息：
  * `usr_ip ip`
  * `usr_port port`
* 打包版本可能缺少相关语料库文件和停词库文件，请在 `lib` 对应文件夹下放置对应文件并修改 `path.conf` 中的别名和路径
