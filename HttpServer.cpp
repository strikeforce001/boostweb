#include <boost/asio.hpp>
#include <iostream>
#include <thread>
#include <string>
#include <vector>

using boost::asio::ip::tcp;

const std::string RESPONSE_200 = "HTTP/1.1 200 OK\r\nContent-Type: text/html\r\n\r\n";
const std::string RESPONSE_404 = "HTTP/1.1 404 Not Found\r\nContent-Type: text/html\r\n\r\n";

class HttpServer {
public:
    HttpServer(boost::asio::io_context& io_context, unsigned short port)
        : acceptor_(io_context, tcp::endpoint(tcp::v4(), port)) {
        startAccept();
    }

private:
    void startAccept() {
        std::shared_ptr<tcp::socket> socket = std::make_shared<tcp::socket>(acceptor_.get_executor().context());
        acceptor_.async_accept(*socket, [this, socket](boost::system::error_code ec) {
            if (!ec) {
                std::thread(&HttpServer::handleRequest, this, socket).detach();
            }
            startAccept();  // Continue accepting new connections
        });
    }

    void handleRequest(std::shared_ptr<tcp::socket> socket) {
        try {
            boost::asio::streambuf request;
            boost::asio::read_until(*socket, request, "\r\n\r\n");

            std::istream request_stream(&request);
            std::string request_line;
            std::getline(request_stream, request_line);

            if (request_line.find("GET") == 0) {
                handleGet(socket);
            } else if (request_line.find("POST") == 0) {
                handlePost(socket);
            } else {
                handleNotFound(socket);
            }
        } catch (std::exception& e) {
            std::cerr << "Error handling request: " << e.what() << std::endl;
        }
    }

    void handleGet(std::shared_ptr<tcp::socket> socket) {
        std::string content = "<html><body><h1>GET request received!</h1></body></html>";
        boost::asio::write(*socket, boost::asio::buffer(RESPONSE_200 + content));
    }

    void handlePost(std::shared_ptr<tcp::socket> socket) {
        std::string content = "<html><body><h1>POST request received!</h1></body></html>";
        boost::asio::write(*socket, boost::asio::buffer(RESPONSE_200 + content));
    }

    void handleNotFound(std::shared_ptr<tcp::socket> socket) {
        std::string content = "<html><body><h1>404 Not Found</h1></body></html>";
        boost::asio::write(*socket, boost::asio::buffer(RESPONSE_404 + content));
    }

    tcp::acceptor acceptor_;
};

int main(int argc, char* argv[]) {
    try {
        if (argc != 2) {
            std::cerr << "Usage: HttpServer <port>" << std::endl;
            return 1;
        }

        boost::asio::io_context io_context;
        unsigned short port = std::atoi(argv[1]);
        HttpServer server(io_context, port);

        std::cout << "Server is running on port " << port << std::endl;
        io_context.run();
    } catch (std::exception& e) {
        std::cerr << "Exception: " << e.what() << std::endl;
    }

    return 0;
}
