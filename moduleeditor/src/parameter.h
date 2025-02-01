#pragma once
#include "arxml_parser/referrable.hpp"
#include "arxml_tags.hpp"
#include "num_param_val.hpp"
#include "text_param_val.hpp"
#include "reference_val.hpp"
#include "reference_def.hpp"

#include <cstdint>

#include <QObject>
#include <QVariant>
#include <QtQml/qqmlregistration.h>
#include <QtQml/qqml.h>
#include <QRegularExpression>
#include <QStringList>

class ParameterType : public QObject
{
    Q_OBJECT
    QML_UNCREATABLE("ParameterType is not instantiable. Use the contained enum.")
    QML_ELEMENT
    QML_SINGLETON
public:
    explicit ParameterType(QObject *parent = nullptr);
    enum Type
    {
        BOOL = 0,
        INTEGER,
        FLOAT,
        STRING,
        LINKER_SYMBOL,
        FUNCTION_NAME,
        ENUMERATION,
        REFERENCE
    };
    Q_ENUM(Type)
};

class ValueStatus : public QObject
{
    Q_OBJECT
    QML_UNCREATABLE("ValueStatus is not instantiable. Use the contained enum.")
    QML_ELEMENT
    QML_SINGLETON
public:
    explicit ValueStatus(QObject *parent = nullptr);
    enum Status
    {
        NOT_INSTANTIATED = 0, /*!< No entry in EcuC. Default value is used. */
        USER_DEFINED,         /*!< Value is set manually by the user and cannot be derived from other sources. */
        DERIVED,              /*!< The value is derived (i.e. from the EcuExtract or another parameter). */
        USER_OVERRIDE,        /*!< The value can be derived, but was overriden manually by the user. */
        NOT_DEFINED,          /*!< The parameter was not found in the BSWMD (this is usually an error and can happen when the bswmd is updated)*/
    };
    Q_ENUM(Status)
};


class Parameter
{
public:
    Parameter(ParameterType::Type type, std::shared_ptr<arx::referrable> bswmd_def, ValueStatus::Status status = ValueStatus::NOT_INSTANTIATED):
        _type(type),
        _status(status),
        _bswmd_def(bswmd_def)
    {
        if(bswmd_def->has_longname())
        {
            _name = QString::fromStdString(std::string(bswmd_def->longname()));
        }
        else
        {
            QString shortname = QString::fromStdString(std::string(bswmd_def->shortname()));

            // Determine the name of the BSW Module
            std::shared_ptr<arx::referrable> current_node = bswmd_def->parent();
            while(current_node != nullptr)
            {
                if(current_node->type() == arx::kModuleDef)
                {
                    // Strip the modulename from the parameter name (if necessary)
                    QString moduleName = QString::fromStdString(std::string(current_node->shortname()));
                    if(shortname.startsWith(moduleName))
                    {
                        shortname = shortname.sliced(moduleName.length());
                    }
                    break;

                }
                current_node = current_node->parent();
            }

            QRegularExpression regexp("[A-Z]*[^A-Z]*");
            QRegularExpressionMatchIterator match = regexp.globalMatch(shortname);
            QStringList list;

            while(match.hasNext())
                list.append(match.next().capturedTexts());
            _name = list.join(' ');
        }
    }

    // Getters
    const QString& name() const { return _name; }
    ParameterType::Type type() const { return _type; }
    ValueStatus::Status status() const { return _status; }
    virtual const QVariant& value() const = 0;
    virtual const QVariant& choices() const { static QVariant result; return result; }
    virtual uint32_t upper_multiplicity() const { return 1u; }
    virtual uint32_t lower_multiplicity() const { return 0u; }
    std::shared_ptr<arx::referrable> bswmd_def() const { return _bswmd_def; }

    // Setters
    void name(const QString& name)  { _name = name; }
    void type(ParameterType::Type type)  { _type = type; }
    void status(ValueStatus::Status status)  { _status = status; }
    virtual void value(const QVariant& value)  = 0;  
protected:
    std::shared_ptr<arx::referrable> _bswmd_def;
    ValueStatus::Status _status;
    QString _name;
private:
    ParameterType::Type _type;
};

class BoolParameter : public Parameter
{
public:
    BoolParameter(std::shared_ptr<arx::referrable> bswmd_def, AutosarGen::num_param_val* ecuc_def = nullptr):
        Parameter(ParameterType::BOOL, bswmd_def),
        _ecuc_def(ecuc_def)
    {
        if(ecuc_def != nullptr)
        {
            _status = ValueStatus::USER_DEFINED;
            if(ecuc_def->value() == 0)
            {
                _value = false;
            }
            else
            {
                _value = true;
            }
        }
        else
        {
            // Container is not instantiated
            _status = ValueStatus::NOT_INSTANTIATED;
            // TODO Determine default value
            _value = false;
        }
    }

    const QVariant& value() const override
    {
        return _value;
    }

    void value(const QVariant& value) override
    {
        _value = value;
    }
private:
    AutosarGen::num_param_val* _ecuc_def;
    QVariant _value;
};

class IntParameter : public Parameter
{
public:
    IntParameter(std::shared_ptr<arx::referrable> bswmd_def, AutosarGen::num_param_val* ecuc_def = nullptr):
        Parameter(ParameterType::INTEGER, bswmd_def),
        _ecuc_def(ecuc_def)
    {
        if(ecuc_def != nullptr)
        {
            _value = static_cast<qlonglong>(ecuc_def->value());
        }
        else
        {
            // Container is not instantiated
            _status = ValueStatus::NOT_INSTANTIATED;
            // TODO Determine default value
            _value = 0;
        }
    }

    const QVariant& value() const override
    {
        return _value;
    }

    void value(const QVariant& value) override
    {
        _value = value;
    }
private:
    AutosarGen::num_param_val* _ecuc_def;
    QVariant _value;
};

class FloatParameter : public Parameter
{
public:
    FloatParameter(std::shared_ptr<arx::referrable> bswmd_def, AutosarGen::num_param_val* ecuc_def = nullptr):
        Parameter(ParameterType::FLOAT, bswmd_def),
        _ecuc_def(ecuc_def)
    {
        if(ecuc_def != nullptr)
        {
            _value = static_cast<double>(ecuc_def->value_floating());
        }
        else
        {
            // Container is not instantiated
            _status = ValueStatus::NOT_INSTANTIATED;
            // TODO Determine default value
            _value = 0.0;
        }
    }

    const QVariant& value() const override
    {
        return _value;
    }

    void value(const QVariant& value) override
    {
        _value = value;
    }

private:
    AutosarGen::num_param_val* _ecuc_def;
    QVariant _value;
};

class LinkerSymbol : public Parameter
{
public:
    LinkerSymbol(std::shared_ptr<arx::referrable> bswmd_def, AutosarGen::text_param_val* ecuc_def = nullptr):
        Parameter(ParameterType::LINKER_SYMBOL, bswmd_def),
        _ecuc_def(ecuc_def)
    {
        if(ecuc_def != nullptr)
        {
            _status = ValueStatus::USER_DEFINED;
            _value = QString::fromStdString(std::string(ecuc_def->value()));
        }
        else
        {
            // Container is not instantiated
            _status = ValueStatus::NOT_INSTANTIATED;
            // TODO Determine default value
            _value = "";
        }
    }

    const QVariant& value() const override
    {
        return _value;
    }

    void value(const QVariant& value) override
    {
        _value = value;
    }
private:
    AutosarGen::text_param_val* _ecuc_def;
    QVariant _value;
};

class FunctionName : public Parameter
{
public:
    FunctionName(std::shared_ptr<arx::referrable> bswmd_def, AutosarGen::text_param_val* ecuc_def = nullptr):
        Parameter(ParameterType::FUNCTION_NAME, bswmd_def),
        _ecuc_def(ecuc_def)
    {
        if(ecuc_def != nullptr)
        {
            _status = ValueStatus::USER_DEFINED;
            _value = QString::fromStdString(std::string(ecuc_def->value()));
        }
        else
        {
            // Container is not instantiated
            _status = ValueStatus::NOT_INSTANTIATED;
            // TODO Determine default value
            _value = "";
        }
    }

    const QVariant& value() const override
    {
        return _value;
    }

    void value(const QVariant& value) override
    {
        _value = value;
    }
private:
    AutosarGen::text_param_val* _ecuc_def;
    QVariant _value;
};

class EnumerationParameter : public Parameter
{
public:
    EnumerationParameter(std::shared_ptr<arx::referrable> bswmd_def, AutosarGen::text_param_val* ecuc_def = nullptr):
        Parameter(ParameterType::ENUMERATION, bswmd_def),
        _ecuc_def(ecuc_def)
    {
        if(ecuc_def != nullptr)
        {
            _status = ValueStatus::USER_DEFINED;
            _value = QString::fromStdString(std::string(ecuc_def->value()));

        }
        else
        {
            // Container is not instantiated
            _status = ValueStatus::NOT_INSTANTIATED;
            // TODO Determine default value
            _value = "";
        }
        for(auto child: bswmd_def->children())
        {
            
            if(child->type() == arx::kEnumLiteralDef)
            {
                literals.append(QString::fromStdString(std::string(child->shortname())));
            }
            _choices = literals;
        }
    }

    const QVariant& value() const override
    {
        return _value;
    }

    const QVariant& choices() const override
    {
        return _choices;
    }

    void value(const QVariant& value) override
    {
        _value = value;
    }
private:
    std::vector<std::string> _literals;
    QStringList literals;
    AutosarGen::text_param_val* _ecuc_def;
    QVariant _value;
    QVariant _choices;
};


class StringParameter : public Parameter
{
public:
    StringParameter(std::shared_ptr<arx::referrable> bswmd_def, AutosarGen::text_param_val* ecuc_def = nullptr):
        Parameter(ParameterType::STRING, bswmd_def),
        _ecuc_def(ecuc_def)

    {
        if(ecuc_def != nullptr)
        {
            _status = ValueStatus::USER_DEFINED;
            _value = QString::fromStdString(std::string(ecuc_def->value()));
        }
        else
        {
            // Container is not instantiated
            _status = ValueStatus::NOT_INSTANTIATED;
            // TODO Determine default value
            _value = "";
        }
    }

    const QVariant& value() const override
    {
        return _value;
    }

    void value(const QVariant& value) override
    {
        _value = value;
    }
private:
    AutosarGen::text_param_val* _ecuc_def;
    QVariant _value;
};

class Reference : public Parameter
{
public:
    Reference(std::shared_ptr<arx::referrable> bswmd_def, const std::vector<AutosarGen::reference_val*>& ecuc_defs):
        Parameter(ParameterType::REFERENCE,  bswmd_def),
        _ecuc_defs(ecuc_defs)
    {
        if(_ecuc_defs.size() > 0)
        {
            _status = ValueStatus::USER_DEFINED;
            if(upper_multiplicity() < 2)
            {
                QString ecucPath = QString::fromStdString(std::string(_ecuc_defs[0]->value()->targetpath()));
                QString link = "<a href=\""+ecucPath+"\">"+ecucPath+"</a>";
                _value = link;
            }
            else
            {   
                QString value = "";
                for(auto& ref: _ecuc_defs)
                {
                    QString ecucPath = QString::fromStdString(std::string(ref->value()->targetpath()));
                    QString link = "<a href=\""+ecucPath+"\">"+ecucPath+"</a>";
                    value.append(link);
                    if(ref != _ecuc_defs.back())
                    {
                        value.append("<br />");
                    }
                }
                _value = value;
            }
        }
        else
        {
            // Container is not instantiated
            _status = ValueStatus::NOT_INSTANTIATED;
            // TODO Determine default value
            _value = "";
        }
    }

    const QVariant& value() const override
    {
        return _value;
    }

    void value(const QVariant& value) override
    {
        _value = value;
    }

    uint32_t upper_multiplicity() const override
    {
        auto reference_def = std::static_pointer_cast<AutosarGen::reference_def>(_bswmd_def);
        return reference_def->upper_multiplicity(); 
    }

    uint32_t lower_multiplicity() const override
    {
        auto reference_def = std::static_pointer_cast<AutosarGen::reference_def>(_bswmd_def);
        return reference_def->lower_multiplicity(); 
    }

private:
    std::vector<AutosarGen::reference_val*> _ecuc_defs;
    QVariant _value;
};