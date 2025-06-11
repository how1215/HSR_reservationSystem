#!/bin/bash

# Git 自動同步腳本（for macOS / Linux）
# 建議儲存成 git-sync.sh，並用 chmod +x git-sync.sh 使其可執行

# 設定變數
BRANCH="main"
MSG=${1:-"Auto update from git-sync.sh"}

echo "📁 [Step 1] 檢查 Git 狀態..."
git status

echo "📥 [Step 2] 將變更加入暫存區..."
git add .

echo "📝 [Step 3] 提交本地變更（訊息：$MSG）..."
git commit -m "$MSG"

echo "🔄 [Step 4] 從遠端拉取最新程式碼（使用 rebase）..."
git pull --rebase origin $BRANCH

echo "🚀 [Step 5] 推送到 GitHub ($BRANCH)..."
git push origin $BRANCH

echo "✅ 同步完成！"
