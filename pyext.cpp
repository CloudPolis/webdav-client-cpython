#include <boost/python/class.hpp>
#include <boost/python/module.hpp>
#include <boost/python/def.hpp>
#include <boost/python/dict.hpp>
#include <boost/python/overloads.hpp>
#include <boost/python/make_constructor.hpp>
#include <boost/python/return_internal_reference.hpp>

#include <webdav/client.hpp>

namespace py = boost::python;

using dict_t = std::map<std::string, std::string>;

auto to_cpp_dict(py::dict& py_dict) -> dict_t {  

    dict_t coll;
    py::list keys = py_dict.keys();  
    for (auto i = 0; i < len(keys); ++i) {  
       boost::python::extract<std::string> extracted_key(keys[i]);  
       if(!extracted_key.check()){  
            std::cout<<"Key invalid, map might be incomplete"<<std::endl;  
            continue;                 
       }  
       std::string key = extracted_key;  
       boost::python::extract<std::string> extracted_val(py_dict[key]);  
       if(!extracted_val.check()){  
       std::cout<<"Value invalid, map might be incomplete"<<std::endl;  
            continue;                 
       }  
       std::string value = extracted_val;  
       coll[key] = value;  
    }  
    return coll;
}

template <class T>
auto to_python_list(std::vector<T> vector) -> py::list {
    typename std::vector<T>::iterator iter;
    boost::python::list list;
    for (iter = vector.begin(); iter != vector.end(); ++iter) {
        list.append(*iter);
    }
    return list;
}

template <class Key, class Value>
auto to_python_dict(std::map<Key, Value> cpp_dict) -> py::dict {

    boost::python::dict py_dict;
    for (auto item : cpp_dict) {
       py_dict[item.first] = item.second; 
    }
    return py_dict;
}

static boost::shared_ptr<WebDAV::Client> makeClass(boost::python::dict& py_dict)
{
    auto options = to_cpp_dict(py_dict);    
    return boost::shared_ptr<WebDAV::Client>{ new WebDAV::Client{ options } };
}

namespace PyWebDAV 
{
    auto list(const WebDAV::Client& client, const std::string& path) -> py::list {
        auto items = client.list(path);
        return to_python_list<std::string>(items);
    }

    auto info(const WebDAV::Client& client, const std::string& path) -> py::dict {
        auto data = client.info(path);
        return to_python_dict<std::string, std::string>(data);
    }

    auto download(const WebDAV::Client& client, const std::string& from, const std::string& to) -> bool {
        return client.download(from, to);
    }

    auto upload(const WebDAV::Client& client, const std::string& from, const std::string& to) -> bool {
        return client.upload(from, to);
    }
}

BOOST_PYTHON_MEMBER_FUNCTION_OVERLOADS(check_overload, WebDAV::Client::check, 0, 1);
BOOST_PYTHON_MEMBER_FUNCTION_OVERLOADS(create_directory_overload, WebDAV::Client::create_directory, 1, 2);

BOOST_PYTHON_MODULE(pywdc)
{
    using namespace WebDAV;
    using namespace boost::python;

    using list_t = std::vector<std::string>;

    class_<Client, boost::noncopyable>("Client", no_init)
        .def("__init__", make_constructor(&makeClass))
        .def("free", &Client::free_size)
        .def("check", &Client::check, check_overload())
        .def("info", PyWebDAV::info)
        .def("clean", &Client::clean)
        .def("is_dir", &Client::is_directory)
        .def("list", PyWebDAV::list)
        .def("mkdir", &Client::create_directory, create_directory_overload())
        .def("move", &Client::move)
        .def("copy", &Client::copy)
        .def("download", PyWebDAV::download)
        .def("upload", PyWebDAV::upload)
    ;
}

