#include "httplib.h"   // cpp-httplib ライブラリ
#include <vector>      // メッセージを格納する std::vector
#include <string>      // 文字列型 std::string
#include <iostream>    // コンソール出力

int main() {
	// ==============================
	// サーバーオブジェクトを作成
	// ==============================
	httplib::Server server;

	// ==============================
	// スレッドプールを1に制限
	// 研修用にシンプル化
	// ==============================
	server.new_task_queue = [] {
		return new httplib::ThreadPool(1);
		};

	// ==============================
	// 送信されたメッセージを保存する場所
	// ==============================
	std::vector<std::string> messages;

	// ==============================
	// POSTリクエスト "/send"
	// クライアントから送られたメッセージを保存
	// ==============================
	server.Post("/send", [&](const httplib::Request& req, httplib::Response& res) {
		// req.body にクライアントから送られた文字列が入る
		messages.push_back(req.body);

		// レスポンスを返す
		res.set_content("OK", "text/plain");
		});

	// ==============================
	// GETリクエスト "/messages"
	// これまでのメッセージをまとめて返す
	// ==============================
	server.Get("/messages", [&](const httplib::Request&, httplib::Response& res) {
		std::string result;

		// messages に溜まった全メッセージを結合
		for (const auto& msg : messages) {
			result += msg + "\n";
		}

		// クライアントに送信
		res.set_content(result, "text/plain");
		});

	// ==============================
	// サーバー起動のメッセージ
	// ==============================
	std::cout << "=== Hello I'm chat server!! ===" << std::endl;

	// ==============================
	// サーバーを 0.0.0.0:8080 で起動
	// ==============================
	server.listen("0.0.0.0", 8080);
}