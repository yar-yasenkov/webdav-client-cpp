#include "stdafx.h"
<<<<<<< HEAD
#include <webdav/Client.hpp>
=======
#include <client.hpp>
>>>>>>> 90cc2f2fe27b0ad5d2cc3b1b79a9a2f33f30d57f
#include "pugiext.hpp"
#include "header.hpp"
#include "request.hpp"
#include "urn.hpp"
#include "fsinfo.hpp"
#include "callback.hpp"

namespace WebDAV
{
<<<<<<< HEAD
	typedef int(* progress_funptr)(void *context, size_t dltotal, size_t dlnow, size_t ultotal, size_t ulnow);

	class ClientImpl : public Client
	{
	public:

		void init() noexcept;

		std::string webdav_hostname;
		std::string webdav_root;
		std::string webdav_login;
		std::string webdav_password;

		std::string proxy_hostname;
		std::string proxy_login;
		std::string proxy_password;

		std::string cert_path;
		std::string key_path;

		std::map<std::string, std::string> options() noexcept;

		bool sync_download(
			std::string remote_file, 
			std::string local_file, 
			callback_t callback = nullptr,
			progress_t progress = nullptr
		) noexcept;

		bool sync_download_to(
			std::string remote_file, 
			char * & buffer_ptr, 
			long long int & buffer_size, 
			callback_t callback = nullptr,
			progress_t progress = nullptr
		) noexcept;

		bool sync_upload(
			std::string remote_file, 
			std::string local_file, 
			callback_t callback = nullptr,
			progress_t progress = nullptr
		) noexcept;

		bool sync_upload_from(
			std::string remote_file, 
			char * buffer_ptr, 
			long long int buffer_size, 
			callback_t callback = nullptr,
			progress_t progress = nullptr
		) noexcept;

		ClientImpl(std::map<std::string, std::string> & options) noexcept;
	};

	
	inline ClientImpl * GetImpl(Client * ptr) { return (ClientImpl *)ptr; }
	inline const ClientImpl * GetImpl(const Client * ptr) { return (const ClientImpl *)ptr; }

	ClientImpl::ClientImpl(std::map<std::string, std::string> & options) noexcept
=======
	Client::Client(std::map<std::string, std::string> & options) noexcept
>>>>>>> 90cc2f2fe27b0ad5d2cc3b1b79a9a2f33f30d57f
	{
		this->webdav_hostname = options["webdav_hostname"];
		this->webdav_root = options["webdav_root"];
		this->webdav_login = options["webdav_login"];
		this->webdav_password = options["webdav_password"];

		this->proxy_hostname = options["proxy_hostname"];
		this->proxy_login = options["proxy_login"];
		this->proxy_password = options["proxy_password"];

		this->cert_path = options["cert_path"];
		this->key_path = options["key_path"];

		this->init();
	}

<<<<<<< HEAD
	void
	ClientImpl::init() noexcept
	{
		curl_global_init(CURL_GLOBAL_DEFAULT);
	}

	std::map<std::string, std::string>
	ClientImpl::options() noexcept
=======
	std::map<std::string, std::string>
	Client::options() noexcept
>>>>>>> 90cc2f2fe27b0ad5d2cc3b1b79a9a2f33f30d57f
	{
		return std::map < std::string, std::string >
		{
			{ "webdav_hostname", this->webdav_hostname },
			{ "webdav_root", this->webdav_root },
			{ "webdav_login", this->webdav_login },
			{ "webdav_password", this->webdav_password },
			{ "proxy_hostname", this->proxy_hostname },
			{ "proxy_login", this->proxy_login },
			{ "proxy_password", this->proxy_password },
			{ "cert_path", this->cert_path },
			{ "key_path", this->key_path },
		};
	}

<<<<<<< HEAD
	bool
	ClientImpl::sync_download(
		std::string remote_file, 
		std::string local_file, 
		callback_t callback, 
		progress_t progress
	) noexcept
	{
		bool is_existed = this->check(remote_file);
		if (!is_existed) return false;

		auto root_urn = Urn(this->webdav_root);
		auto file_urn = root_urn + remote_file;

		std::ofstream file_stream(local_file, std::ios::binary);

		Request request(this->options());

		auto url = this->webdav_hostname + file_urn.quote(request.handle);

		request.set(CURLOPT_CUSTOMREQUEST, "GET");
		request.set(CURLOPT_URL, url.c_str());
		request.set(CURLOPT_HEADER, 0L);
		request.set(CURLOPT_WRITEDATA, (size_t)&file_stream);
		request.set(CURLOPT_WRITEFUNCTION, (size_t)Callback::Write::file);
		if (progress != nullptr) {
			request.set(CURLOPT_XFERINFOFUNCTION, (size_t)progress.target<progress_funptr>());
			request.set(CURLOPT_NOPROGRESS, 0L);
		}

		bool is_performed = request.perform();

		if (callback != nullptr) callback(is_performed);
		return is_performed;
	}

	bool
	ClientImpl::sync_download_to(
		std::string remote_file, 
		char * & buffer_ptr, 
		long long int & buffer_size, 
		callback_t callback, 
		progress_t progress
	) noexcept
	{
		if (buffer_size == 0) return false;

		bool is_existed = this->check(remote_file);
		if (!is_existed) return false;

		auto root_urn = Urn(this->webdav_root);
		auto file_urn = root_urn + remote_file;

		Data data = { 0, 0, 0 };

		Request request(this->options());

		auto url = this->webdav_hostname + file_urn.quote(request.handle);

		request.set(CURLOPT_CUSTOMREQUEST, "GET");
		request.set(CURLOPT_URL, url.c_str());
		request.set(CURLOPT_HEADER, 0L);
		request.set(CURLOPT_WRITEDATA, (size_t)&data);
		request.set(CURLOPT_WRITEFUNCTION, (size_t)Callback::Append::buffer);
		if (progress != nullptr) {
			request.set(CURLOPT_XFERINFOFUNCTION, (size_t)progress.target<progress_funptr>());
			request.set(CURLOPT_NOPROGRESS, 0L);
		}

		bool is_performed = request.perform();
		if (callback != nullptr) callback(is_performed);
		if (!is_performed) return false;

		buffer_ptr = data.buffer;
		buffer_size = data.size;
		return true;
	}

	bool
	ClientImpl::sync_upload(
		std::string remote_file, 
		std::string local_file, 
		callback_t callback, 
		progress_t progress
	) noexcept
	{
		bool is_existed = FileInfo::exists(local_file);
		if (!is_existed) return false;

		auto root_urn = Urn(this->webdav_root);
		auto file_urn = root_urn + remote_file;

		std::ifstream file_stream(local_file, std::ios::binary);
		auto size = FileInfo::size(local_file);

		Request request(this->options());

		auto url = this->webdav_hostname + file_urn.quote(request.handle);

		Data response = { 0, 0, 0 };

		request.set(CURLOPT_UPLOAD, 1L);
		request.set(CURLOPT_URL, url.c_str());
		request.set(CURLOPT_READDATA, (size_t)&file_stream);
		request.set(CURLOPT_READFUNCTION, (size_t)Callback::Read::file);
		request.set(CURLOPT_INFILESIZE_LARGE, (curl_off_t)size);
		request.set(CURLOPT_BUFFERSIZE, (long)Client::buffer_size);
		request.set(CURLOPT_WRITEDATA, (size_t)&response);
		request.set(CURLOPT_WRITEFUNCTION, (size_t)Callback::Append::buffer);
		if (progress != nullptr) {
			request.set(CURLOPT_XFERINFOFUNCTION, (size_t)progress.target<progress_funptr>());
			request.set(CURLOPT_NOPROGRESS, 0L);
		}

		bool is_performed = request.perform();

		if (callback != nullptr) callback(is_performed);
		return is_performed;
	}

	bool
	ClientImpl::sync_upload_from(
		std::string remote_file, 
		char * buffer, 
		long long int buffer_size, 
		callback_t callback, 
		progress_t progress
	) noexcept
	{
		auto root_urn = Urn(this->webdav_root);
		auto file_urn = root_urn + remote_file;

		Data data = { buffer, 0, buffer_size };

		Request request(this->options());

		auto url = this->webdav_hostname + file_urn.quote(request.handle);

		Data response = { 0, 0, 0 };

		request.set(CURLOPT_UPLOAD, 1L);
		request.set(CURLOPT_URL, url.c_str());
		request.set(CURLOPT_READDATA, (size_t)&data);
		request.set(CURLOPT_READFUNCTION, (size_t)Callback::Read::buffer);
		request.set(CURLOPT_INFILESIZE_LARGE, (curl_off_t)buffer_size);
		request.set(CURLOPT_BUFFERSIZE, (long)buffer_size);
		request.set(CURLOPT_WRITEDATA, (size_t)&response);
		request.set(CURLOPT_WRITEFUNCTION, (size_t)Callback::Append::buffer);
		if (progress != nullptr) {
			request.set(CURLOPT_XFERINFOFUNCTION, (size_t)progress.target<progress_funptr>());
			request.set(CURLOPT_NOPROGRESS, 0L);
		}
	
		bool is_performed = request.perform();

		if (callback != nullptr) callback(is_performed);
		return is_performed;
	}


	Client * Client::Init(std::map<std::string, std::string> & options)
	{
		return new ClientImpl(options);
	}

	Client::~Client() noexcept
	{
		curl_global_cleanup();
	}

	
=======
	Client::~Client() noexcept
	{
		curl_global_cleanup();
	}

	void
	Client::init() noexcept
	{
		curl_global_init(CURL_GLOBAL_DEFAULT);
	}

>>>>>>> 90cc2f2fe27b0ad5d2cc3b1b79a9a2f33f30d57f
	long long
	Client::free_size() noexcept
	{
		Header header = {
				"Accept: */*",
				"Depth: 0",
				"Content-Type: text/xml"
		};

		pugi::xml_document document;
		auto propfind = document.append_child("D:propfind");
		propfind.append_attribute("xmlns:D") = "DAV:";

		auto prop = propfind.append_child("D:prop");
		prop.append_child("D:quota-available-bytes");
		prop.append_child("D:quota-used-bytes");

		auto document_print = pugi::node_to_string(document);
		size_t size = document_print.length() * sizeof((document_print.c_str())[0]);

		Data data = { 0, 0, 0 };

<<<<<<< HEAD
		Request request(GetImpl(this)->options());
=======
		Request request(this->options());
>>>>>>> 90cc2f2fe27b0ad5d2cc3b1b79a9a2f33f30d57f

		request.set(CURLOPT_CUSTOMREQUEST, "PROPFIND");
		request.set(CURLOPT_HTTPHEADER, (struct curl_slist *)header.handle);
		request.set(CURLOPT_POSTFIELDS, document_print.c_str());
		request.set(CURLOPT_POSTFIELDSIZE, (long)size);
		request.set(CURLOPT_HEADER, 0);
		request.set(CURLOPT_WRITEDATA, (size_t)&data);
		request.set(CURLOPT_WRITEFUNCTION, (size_t)Callback::Append::buffer);

		auto is_performed = request.perform();
		if (!is_performed) return 0;

		document.load_buffer(data.buffer, (size_t)data.size);

		pugi::xml_node multistatus = document.select_single_node("d:multistatus").node();
		pugi::xml_node response = multistatus.select_single_node("d:response").node();
		pugi::xml_node propstat = response.select_single_node("d:propstat").node();
		prop = propstat.select_single_node("d:prop").node();
		pugi::xml_node quota_available_bytes = prop.select_single_node("d:quota-available-bytes").node();
		std::string free_size_text = quota_available_bytes.first_child().value();

		auto free_size = atol(free_size_text.c_str());
		return free_size;
	}

	bool
	Client::check(std::string remote_resource) noexcept
	{
<<<<<<< HEAD
		auto clientImpl = GetImpl(this);
		auto root_urn = Urn(clientImpl->webdav_root);
=======
		auto root_urn = Urn(this->webdav_root);
>>>>>>> 90cc2f2fe27b0ad5d2cc3b1b79a9a2f33f30d57f
		auto resource_urn = root_urn + remote_resource;

		Header header = {
				"Accept: */*",
				"Depth: 1"
		};

		Data data = { 0, 0, 0 };

<<<<<<< HEAD
		Request request(clientImpl->options());

		auto url = clientImpl->webdav_hostname + resource_urn.quote(request.handle);
=======
		Request request(this->options());

		auto url = this->webdav_hostname + resource_urn.quote(request.handle);
>>>>>>> 90cc2f2fe27b0ad5d2cc3b1b79a9a2f33f30d57f

		request.set(CURLOPT_CUSTOMREQUEST, "PROPFIND");
		request.set(CURLOPT_URL, url.c_str());
		request.set(CURLOPT_HTTPHEADER, (struct curl_slist *)header.handle);
		request.set(CURLOPT_WRITEDATA, (size_t)&data);
		request.set(CURLOPT_WRITEFUNCTION, (size_t)Callback::Append::buffer);

		return request.perform();
	}

	std::map<std::string, std::string>
	Client::info(std::string remote_resource) noexcept
	{
<<<<<<< HEAD
		auto clientImpl = GetImpl(this);
		auto root_urn = Urn(clientImpl->webdav_root);
=======
		auto root_urn = Urn(this->webdav_root);
>>>>>>> 90cc2f2fe27b0ad5d2cc3b1b79a9a2f33f30d57f
		auto target_urn = root_urn + remote_resource;

		Header header = {
				"Accept: */*",
				"Depth: 1"
		};

		Data data = { 0, 0, 0 };

<<<<<<< HEAD
		Request request(clientImpl->options());

		auto url = clientImpl->webdav_hostname + target_urn.quote(request.handle);
=======
		Request request(this->options());

		auto url = this->webdav_hostname + target_urn.quote(request.handle);
>>>>>>> 90cc2f2fe27b0ad5d2cc3b1b79a9a2f33f30d57f

		request.set(CURLOPT_CUSTOMREQUEST, "PROPFIND");
		request.set(CURLOPT_URL, url.c_str());
		request.set(CURLOPT_HTTPHEADER, (struct curl_slist *)header.handle);
		request.set(CURLOPT_WRITEDATA, (size_t)&data);
		request.set(CURLOPT_WRITEFUNCTION, (size_t)Callback::Append::buffer);

		bool is_performed = request.perform();

		if (!is_performed) return std::map<std::string, std::string>();

		pugi::xml_document document;
		document.load_buffer(data.buffer, (size_t)data.size);
		auto multistatus = document.select_single_node("d:multistatus").node();
		auto responses = multistatus.select_nodes("d:response");
		for (auto response : responses)
		{
			pugi::xml_node href = response.node().select_single_node("d:href").node();
			std::string encode_file_name = href.first_child().value();
			std::string resource_path = curl_unescape(encode_file_name.c_str(), (int)encode_file_name.length());
			auto target_path = target_urn.path();
<<<<<<< HEAD
			auto target_path_without_sep = std::string(target_path, 0, target_path.rfind("/")+1);
			auto resource_path_without_sep = std::string(resource_path, 0, resource_path.rfind("/")+1);
=======
			auto target_path_without_sep = std::string(target_path, 0, target_path.find_last_not_of("/")+1);
			auto resource_path_without_sep = std::string(resource_path, 0, resource_path.find_last_not_of("/")+1);
>>>>>>> 90cc2f2fe27b0ad5d2cc3b1b79a9a2f33f30d57f
			if (resource_path_without_sep.compare(target_path_without_sep) == 0) {
				auto propstat = response.node().select_single_node("d:propstat").node();
				auto prop = propstat.select_single_node("d:prop").node();
				auto creation_date = prop.select_single_node("d:creationdate").node();
				auto display_name = prop.select_single_node("d:displayname").node();
				auto content_length = prop.select_single_node("d:getcontentlength").node();
				auto modified_date = prop.select_single_node("d:getlastmodified").node();
				auto resource_type = prop.select_single_node("d:resourcetype").node();

				std::map<std::string, std::string> information = {
						{"created", creation_date.first_child().value()},
						{"name", display_name.first_child().value()},
						{"size", content_length.first_child().value()},
						{"modified", modified_date.first_child().value()},
						{"type", resource_type.first_child().name()}
				};

				return information;
			}
		}

		return std::map<std::string, std::string>();
	}

	bool 
	Client::is_dir(std::string remote_resource) noexcept
	{
		auto information = this->info(remote_resource);
		auto resource_type = information["type"];
		bool is_directory = resource_type.compare("d:collection") == 0;
		return is_directory;
	}

	std::vector<std::string>
	Client::list(std::string remote_directory) noexcept
	{
<<<<<<< HEAD
		auto clientImpl = GetImpl(this);
=======
>>>>>>> 90cc2f2fe27b0ad5d2cc3b1b79a9a2f33f30d57f
		bool is_existed = this->check(remote_directory);
		if (!is_existed) return std::vector<std::string>();

		bool is_directory = this->is_dir(remote_directory);
		if (!is_directory) return std::vector<std::string>();

<<<<<<< HEAD
		auto target_urn = Urn(clientImpl->webdav_root) + remote_directory;
=======
		auto target_urn = Urn(this->webdav_root) + remote_directory;
>>>>>>> 90cc2f2fe27b0ad5d2cc3b1b79a9a2f33f30d57f

		Header header = {
				"Accept: */*",
				"Depth: 1"
		};

		Data data = { 0, 0, 0 };

<<<<<<< HEAD
		Request request(clientImpl->options());

		auto url = clientImpl->webdav_hostname + target_urn.quote(request.handle);
=======
		Request request(this->options());

		auto url = this->webdav_hostname + target_urn.quote(request.handle);
>>>>>>> 90cc2f2fe27b0ad5d2cc3b1b79a9a2f33f30d57f

		request.set(CURLOPT_CUSTOMREQUEST, "PROPFIND");
		request.set(CURLOPT_URL, url.c_str());
		request.set(CURLOPT_HTTPHEADER, (struct curl_slist *)header.handle);
		request.set(CURLOPT_HEADER, 0);
		request.set(CURLOPT_WRITEDATA, (size_t)&data);
		request.set(CURLOPT_WRITEFUNCTION, (size_t)Callback::Append::buffer);

		bool is_performed = request.perform();

		if (!is_performed) return std::vector<std::string>();

		std::vector<std::string> resources;

		pugi::xml_document document;
		document.load_buffer(data.buffer, (size_t)data.size);
		auto multistatus = document.select_single_node("d:multistatus").node();
		auto responses = multistatus.select_nodes("d:response");
		for(auto response : responses)
		{
			pugi::xml_node href = response.node().select_single_node("d:href").node();
			std::string encode_file_name = href.first_child().value();
			std::string resource_path = curl_unescape(encode_file_name.c_str(), (int) encode_file_name.length());
			auto target_path = target_urn.path();
<<<<<<< HEAD
			auto target_path_without_sep = std::string(target_path, 0, target_path.rfind("/")+1);
			auto resource_path_without_sep = std::string(resource_path, 0, resource_path.rfind("/")+1);
=======
			auto target_path_without_sep = std::string(target_path, 0, target_path.find_last_not_of("/")+1);
			auto resource_path_without_sep = std::string(resource_path, 0, resource_path.find_last_not_of("/")+1);
>>>>>>> 90cc2f2fe27b0ad5d2cc3b1b79a9a2f33f30d57f
			if (resource_path_without_sep.compare(target_path_without_sep) == 0) continue;
			Urn resource_urn(resource_path);
			resources.push_back(resource_urn.name());
		}

		return resources;
	}

<<<<<<< HEAD
	bool Client::download(
		std::string remote_file, 
		std::string local_file, 
		progress_t progress
	) noexcept
	{
		auto clientImpl = GetImpl(this);
		return clientImpl->sync_download(remote_file, local_file, nullptr, progress);
	}

	void
	Client::async_download(
		std::string remote_file, 
		std::string local_file, 
		callback_t callback, 
		progress_t progress
	) noexcept
	{
		auto clientImpl = GetImpl(this);
		std::thread downloading([&](){ clientImpl->sync_download(remote_file, local_file, callback, progress); });
=======
	bool
	Client::sync_download(std::string remote_file, std::string local_file, std::function<void(bool)> callback) noexcept
	{
		bool is_existed = this->check(remote_file);
		if (!is_existed) return false;

		auto root_urn = Urn(this->webdav_root);
		auto file_urn = root_urn + remote_file;

		std::ofstream file_stream(local_file, std::ios::binary);

		Request request(this->options());

		auto url = this->webdav_hostname + file_urn.quote(request.handle);

		request.set(CURLOPT_CUSTOMREQUEST, "GET");
		request.set(CURLOPT_URL, url.c_str());
		request.set(CURLOPT_HEADER, 0L);
		request.set(CURLOPT_WRITEDATA, (size_t)&file_stream);
		request.set(CURLOPT_WRITEFUNCTION, (size_t)Callback::Write::file);

		bool is_performed = request.perform();

		if (callback != nullptr) callback(is_performed);
		return is_performed;
	}

	bool Client::download(std::string remote_file, std::string local_file) noexcept
	{
		return this->sync_download(remote_file, local_file, nullptr);
	}

	void
	Client::async_download(std::string remote_file, std::string local_file, std::function<void(bool)> callback) noexcept
	{
		std::thread downloading([&](){ this->sync_download(remote_file, local_file, callback); });
>>>>>>> 90cc2f2fe27b0ad5d2cc3b1b79a9a2f33f30d57f
		downloading.join();
	}

	bool
<<<<<<< HEAD
	Client::download_to(
		std::string remote_file, 
		char * & buffer_ptr, 
		long long int & buffer_size, 
		progress_t progress
	) noexcept
	{
		auto clientImpl = GetImpl(this);
		return clientImpl->sync_download_to(remote_file, buffer_ptr, buffer_size, nullptr, progress);
	}

	void
	Client::async_download_to(
		std::string remote_file,
		char * & buffer_ptr, 
		long long int & buffer_size, 
		callback_t callback, 
		progress_t progress
	) noexcept
	{
		auto clientImpl = GetImpl(this);
		std::thread downloading([&](){ clientImpl->sync_download_to(remote_file, buffer_ptr, buffer_size, callback, progress); });
=======
	Client::sync_download_to(std::string remote_file, char * & buffer_ptr, long long int & buffer_size, std::function<void(bool)> callback) noexcept
	{
		if (buffer_size == 0) return false;

		bool is_existed = this->check(remote_file);
		if (!is_existed) return false;

		auto root_urn = Urn(this->webdav_root);
		auto file_urn = root_urn + remote_file;

		Data data = { 0, 0, 0 };

		Request request(this->options());

		auto url = this->webdav_hostname + file_urn.quote(request.handle);

		request.set(CURLOPT_CUSTOMREQUEST, "GET");
		request.set(CURLOPT_URL, url.c_str());
		request.set(CURLOPT_HEADER, 0L);
		request.set(CURLOPT_WRITEDATA, (size_t)&data);
		request.set(CURLOPT_WRITEFUNCTION, (size_t)Callback::Append::buffer);

		bool is_performed = request.perform();
		if (callback != nullptr) callback(is_performed);
		if (!is_performed) return false;

		buffer_ptr = data.buffer;
		buffer_size = data.size;
		return true;
	}

	bool
	Client::download_to(std::string remote_file, char * & buffer_ptr, long long int & buffer_size) noexcept
	{
		return sync_download_to(remote_file, buffer_ptr, buffer_size, nullptr);
	}

	void
	Client::async_download_to(std::string remote_file, char * & buffer_ptr, long long int & buffer_size, std::function<void(bool)> callback) noexcept
	{
		std::thread downloading([&](){ this->sync_download_to(remote_file, buffer_ptr, buffer_size, callback); });
>>>>>>> 90cc2f2fe27b0ad5d2cc3b1b79a9a2f33f30d57f
		downloading.join();
	}

	bool
	Client::create_directory(std::string remote_directory, bool recursive) noexcept
	{
<<<<<<< HEAD
		auto clientImpl = GetImpl(this);
=======
>>>>>>> 90cc2f2fe27b0ad5d2cc3b1b79a9a2f33f30d57f
		bool is_existed = this->check(remote_directory);
		if (is_existed) return true;

		bool resource_is_dir = true;
		Urn directory_urn(remote_directory, resource_is_dir);

		if (recursive) {
			auto remote_parent_directory = directory_urn.parent();
			bool is_created = this->create_directory(remote_parent_directory, true);
			if (!is_created) return false;
		}

		Header header = {
				"Accept: */*",
				"Connection: Keep-Alive"
		};

<<<<<<< HEAD
		Request request(clientImpl->options());

		auto url = clientImpl->webdav_hostname + directory_urn.quote(request.handle);
=======
		Request request(this->options());

		auto url = this->webdav_hostname + directory_urn.quote(request.handle);
>>>>>>> 90cc2f2fe27b0ad5d2cc3b1b79a9a2f33f30d57f

		request.set(CURLOPT_CUSTOMREQUEST, "MKCOL");
		request.set(CURLOPT_URL, url.c_str());
		request.set(CURLOPT_HTTPHEADER, (struct curl_slist *)header.handle);

		return request.perform();
	}

<<<<<<< HEAD
	bool 
	Client::move(std::string remote_source_resource, std::string remote_destination_resource) noexcept
	{
		auto clientImpl = GetImpl(this);
		bool is_existed = this->check(remote_source_resource);
		if (!is_existed) return false;

		Urn root_urn(clientImpl->webdav_root);
=======
	bool Client::move(std::string remote_source_resource, std::string remote_destination_resource) noexcept
	{
		bool is_existed = this->check(remote_source_resource);
		if (!is_existed) return false;

		Urn root_urn(this->webdav_root);
>>>>>>> 90cc2f2fe27b0ad5d2cc3b1b79a9a2f33f30d57f
		auto source_resource_urn = root_urn + remote_source_resource;
		auto destination_resource_urn = root_urn + remote_destination_resource;

		Header header = {
				"Accept: */*",
				"Destination: " + destination_resource_urn.path()
		};

<<<<<<< HEAD
		Request request(clientImpl->options());

		auto url = clientImpl->webdav_hostname + source_resource_urn.quote(request.handle);
=======
		Request request(this->options());

		auto url = this->webdav_hostname + source_resource_urn.quote(request.handle);
>>>>>>> 90cc2f2fe27b0ad5d2cc3b1b79a9a2f33f30d57f

		request.set(CURLOPT_CUSTOMREQUEST, "MOVE");
		request.set(CURLOPT_URL, url.c_str());
		request.set(CURLOPT_HTTPHEADER, (struct curl_slist *)header.handle);

		return request.perform();
	}

<<<<<<< HEAD
	bool 
	Client::copy(std::string remote_source_resource, std::string remote_destination_resource) noexcept
	{
		auto clientImpl = GetImpl(this);
		bool is_existed = this->check(remote_source_resource);
		if (!is_existed) return false;

		Urn root_urn(clientImpl->webdav_root);
=======
	bool Client::copy(std::string remote_source_resource, std::string remote_destination_resource) noexcept
	{
		bool is_existed = this->check(remote_source_resource);
		if (!is_existed) return false;

		Urn root_urn(this->webdav_root);
>>>>>>> 90cc2f2fe27b0ad5d2cc3b1b79a9a2f33f30d57f
		auto source_resource_urn = root_urn + remote_source_resource;
		auto destination_resource_urn = root_urn + remote_destination_resource;

		Header header = {
				"Accept: */*",
				"Destination: " + destination_resource_urn.path()
		};

<<<<<<< HEAD
		Request request(clientImpl->options());

		auto url = clientImpl->webdav_hostname + source_resource_urn.quote(request.handle);
=======
		Request request(this->options());

		auto url = this->webdav_hostname + source_resource_urn.quote(request.handle);
>>>>>>> 90cc2f2fe27b0ad5d2cc3b1b79a9a2f33f30d57f

		request.set(CURLOPT_CUSTOMREQUEST, "COPY");
		request.set(CURLOPT_URL, url.c_str());
		request.set(CURLOPT_HTTPHEADER, (struct curl_slist *)header.handle);

		return request.perform();
	}

	bool
<<<<<<< HEAD
	Client::upload(
		std::string remote_file,
		std::string local_file,
		progress_t progress
	) noexcept
	{
		auto clientImpl = GetImpl(this);
		return clientImpl->sync_upload(remote_file, local_file, nullptr, progress);
	}

	void
	Client::async_upload(
		std::string remote_file, 
		std::string local_file, 
		callback_t callback, 
		progress_t progress
	) noexcept
	{
		auto clientImpl = GetImpl(this);
		std::thread uploading([&](){ clientImpl->sync_upload(remote_file, local_file, callback, progress); });
=======
	Client::sync_upload(std::string remote_file, std::string local_file, std::function<void(bool)> callback) noexcept
	{
		bool is_existed = FileInfo::exists(local_file);
		if (!is_existed) return false;

		auto root_urn = Urn(this->webdav_root);
		auto file_urn = root_urn + remote_file;

		std::ifstream file_stream(local_file, std::ios::binary);
		auto size = FileInfo::size(local_file);

		Request request(this->options());

		auto url = this->webdav_hostname + file_urn.quote(request.handle);

		Data response = { 0, 0, 0 };

		request.set(CURLOPT_UPLOAD, 1L);
		request.set(CURLOPT_URL, url.c_str());
		request.set(CURLOPT_READDATA, (size_t)&file_stream);
		request.set(CURLOPT_READFUNCTION, (size_t)Callback::Read::file);
		request.set(CURLOPT_INFILESIZE_LARGE, (curl_off_t)size);
		request.set(CURLOPT_BUFFERSIZE, (long)Client::buffer_size);
		request.set(CURLOPT_WRITEDATA, (size_t)&response);
		request.set(CURLOPT_WRITEFUNCTION, (size_t)Callback::Append::buffer);

		bool is_performed = request.perform();

		if (callback != nullptr) callback(is_performed);
		return is_performed;
	}


	bool
	Client::upload(std::string remote_file, std::string local_file) noexcept
	{
		return this->sync_upload(remote_file, local_file, nullptr);
	}

	void
	Client::async_upload(std::string remote_file, std::string local_file, std::function<void(bool)> callback) noexcept
	{
		std::thread uploading([&](){ this->sync_upload(remote_file, local_file, callback); });
>>>>>>> 90cc2f2fe27b0ad5d2cc3b1b79a9a2f33f30d57f
		uploading.join();
	}

	bool
<<<<<<< HEAD
	Client::upload_from(
		std::string remote_file, 
		char * buffer_ptr,
		long long int buffer_size, 
		progress_t progress
	) noexcept
	{
		auto clientImpl = GetImpl(this);
		return clientImpl->sync_upload_from(remote_file, buffer_ptr, buffer_size, nullptr, progress);
	}

	void
	Client::async_upload_from(
		std::string remote_file, 
		char * buffer_ptr, 
		long long int buffer_size, 
		callback_t callback, 
		progress_t progress
	) noexcept
	{
		auto clientImpl = GetImpl(this);
		std::thread uploading([&](){ clientImpl->sync_upload_from(remote_file, buffer_ptr, buffer_size, callback, progress); });
=======
	Client::sync_upload_from(std::string remote_file, char* buffer, long long int buffer_size, std::function<void(bool)> callback) noexcept
	{
		auto root_urn = Urn(this->webdav_root);
		auto file_urn = root_urn + remote_file;

		Data data = { buffer, 0, buffer_size };

		Request request(this->options());

		auto url = this->webdav_hostname + file_urn.quote(request.handle);

		Data response = { 0, 0, 0 };

		request.set(CURLOPT_UPLOAD, 1L);
		request.set(CURLOPT_URL, url.c_str());
		request.set(CURLOPT_READDATA, (size_t)&data);
		request.set(CURLOPT_READFUNCTION, (size_t)Callback::Read::buffer);
		request.set(CURLOPT_INFILESIZE_LARGE, (curl_off_t)buffer_size);
		request.set(CURLOPT_BUFFERSIZE, (long)buffer_size);
		request.set(CURLOPT_WRITEDATA, (size_t)&response);
		request.set(CURLOPT_WRITEFUNCTION, (size_t)Callback::Append::buffer);
	
		bool is_performed = request.perform();

		if (callback != nullptr) callback(is_performed);
		return is_performed;
	}

	bool
	Client::upload_from(std::string remote_file, char* buffer_ptr, long long int buffer_size) noexcept
	{
		return this->sync_upload_from(remote_file, buffer_ptr, buffer_size, nullptr);
	}

	void
	Client::async_upload_from(std::string remote_file, char* buffer_ptr, long long int buffer_size, std::function<void(bool)> callback) noexcept
	{
		std::thread uploading([&](){ this->sync_upload_from(remote_file, buffer_ptr, buffer_size, callback); });
>>>>>>> 90cc2f2fe27b0ad5d2cc3b1b79a9a2f33f30d57f
		uploading.join();
	}

	bool
	Client::clean(std::string remote_resource) noexcept
	{
<<<<<<< HEAD
		auto clientImpl = GetImpl(this);
		bool is_existed = this->check(remote_resource);
		if (!is_existed) return true;

		auto root_urn = Urn(clientImpl->webdav_root);
=======
		bool is_existed = this->check(remote_resource);
		if (!is_existed) return true;

		auto root_urn = Urn(this->webdav_root);
>>>>>>> 90cc2f2fe27b0ad5d2cc3b1b79a9a2f33f30d57f
		auto resource_urn = root_urn + remote_resource;

		Header header = {
				"Accept: */*",
				"Connection: Keep-Alive"
		};

<<<<<<< HEAD
		Request request(clientImpl->options());

		auto url = clientImpl->webdav_hostname + resource_urn.quote(request.handle);
=======
		Request request(this->options());

		auto url = this->webdav_hostname + resource_urn.quote(request.handle);
>>>>>>> 90cc2f2fe27b0ad5d2cc3b1b79a9a2f33f30d57f

		request.set(CURLOPT_CUSTOMREQUEST, "DELETE");
		request.set(CURLOPT_URL, url.c_str());
		request.set(CURLOPT_HTTPHEADER, (struct curl_slist *)header.handle);

		return request.perform();
	}
<<<<<<< HEAD
}
=======
}
>>>>>>> 90cc2f2fe27b0ad5d2cc3b1b79a9a2f33f30d57f