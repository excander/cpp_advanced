#pragma once
#include <iostream>

enum class Error
{
    NoError,
    CorruptedArchive
};

class Serializer
{
    std::ostream& out_;
    static constexpr char Separator = ' ';
public:
    explicit Serializer(std::ostream& out)
        : out_(out)
    {}

    template <class T>
    Error save(T& object){
        return object.serialize(*this);
    }

    template <class... ArgsT>
    Error operator()(ArgsT... args){
        return process(args...);
    }
    
private:
    template <class T, class... Args>
    Error process(T val, Args... args){ 
        process(val);
        out_ << Separator;
        return process(args...);
    }

    Error process(bool val){
        if (val == true)
            out_ << "true";
        else if (val == false)
            out_ << "false";

        return Error::NoError;
    }

    Error process(uint64_t val){ 
        out_ << val;

        return Error::NoError;
    }
};

class Deserializer
{
    std::istream& in_;
    static constexpr char Separator = ' ';
public:
    explicit Deserializer(std::istream& in)
        : in_(in)
    {}

    template <class T>
    Error load(T& object){
        return object.serialize(*this);
    }

    template <class... ArgsT>
    Error operator()(ArgsT&&... args){
        return process(std::forward<ArgsT>(args)...);
    }
    
private:
    template <class T, class... Args>
    Error process(T&& val, Args&&... args){ 
        if (process(val) == Error::CorruptedArchive)
            return Error::CorruptedArchive;
        return process(std::forward<Args>(args)...);
    }

    Error process(bool& value){
        std::string text;
        in_ >> text;

        if (text == "true")
            value = true;
        else if (text == "false")
            value = false;
        else
            return Error::CorruptedArchive;

        return Error::NoError;
    }

    Error process(uint64_t& val){ 
        std::string text;
        in_ >> text;

        if (text == "")
            return Error::CorruptedArchive;
        for (auto &c: text)
            if (!isdigit(c))
                return Error::CorruptedArchive;

        val = std::stoi(text);

        return Error::NoError;
    }
};


