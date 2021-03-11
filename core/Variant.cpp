//
// Copyright (C) 2021 Scidian Software - All Rights Reserved
//
// Unauthorized Copying of this File, via Any Medium is Strictly Prohibited
// Proprietary and Confidential
// Written by Stephens Nunnally <stevinz@gmail.com> - Thu Mar 04 2021
//
//
#include "3rd_party/handmade_math.h"
#include "core/Containers.h"
#include "core/Math.h"
#include "Variant.h"


//####################################################################################
//##    Overload Operators
//####################################################################################
DrVariant& DrVariant::operator=(const DrVariant& other) {
    m_value = other.value();
    m_type = other.getType();
    return *this;
}

bool DrVariant::operator==(DrVariant& other) {
    if (m_type != other.getType()) return false;
    switch (m_type) {
        case Variant_Type::Bool:            return (toBool() ==     other.toBool());
        case Variant_Type::Int:             return (toInt() ==      other.toInt());
        case Variant_Type::Long:            return (toLong() ==     other.toLong());
        case Variant_Type::Unsigned_Int:    return (toUInt() ==     other.toUInt());
        case Variant_Type::Float:           return (FuzzyCompare(toFloat(),  other.toFloat()));
        case Variant_Type::Double:          return (FuzzyCompare(toDouble(), other.toDouble()));
        case Variant_Type::String:          return (toString() ==   other.toString());
        case Variant_Type::Point:           return (toPoint() ==    other.toPoint());
        case Variant_Type::PointF:          return (toPointF() ==   other.toPointF());
        case Variant_Type::Vec2:            return (toVec2() ==     other.toBool());
        case Variant_Type::Vec3:            return (toVec3() ==     other.toBool());
        case Variant_Type::Vector:          return false;
        case Variant_Type::Unknown:         return false;
    }
    return false;
}


//####################################################################################
//##    Primitive Data Type Returns
//####################################################################################
bool DrVariant::toBool() {
    try { return nonstd::any_cast<bool>(m_value); }
    catch (const nonstd::bad_any_cast &) {
        try { return static_cast<bool>(nonstd::any_cast<int>(m_value)); }
        catch (const nonstd::bad_any_cast &) {
            try { return static_cast<bool>(nonstd::any_cast<float>(m_value)); }
            catch (const nonstd::bad_any_cast &) {
                try { return static_cast<bool>(nonstd::any_cast<double>(m_value)); }
                catch (const nonstd::bad_any_cast &) {
                    try { return static_cast<bool>(nonstd::any_cast<float>(m_value)); }
                    catch (const nonstd::bad_any_cast &) {
                        return false; }
                }
            }
        }
    }
}

double DrVariant::toDouble() {
    try { return nonstd::any_cast<double>(m_value); }
    catch (const nonstd::bad_any_cast &) {
        try { return static_cast<double>(nonstd::any_cast<float>(m_value)); }
        catch (const nonstd::bad_any_cast &) {
            try { return static_cast<double>(nonstd::any_cast<int>(m_value)); }
            catch (const nonstd::bad_any_cast &) {
                try { return static_cast<double>(nonstd::any_cast<long>(m_value)); }
                catch (const nonstd::bad_any_cast &) {
                    try { return static_cast<double>(nonstd::any_cast<unsigned int>(m_value)); }
                    catch (const nonstd::bad_any_cast &) {
                        try { return static_cast<double>(nonstd::any_cast<unsigned long>(m_value)); }
                        catch (const nonstd::bad_any_cast &) {
                            try { return static_cast<double>(nonstd::any_cast<size_t>(m_value)); }
                            catch (const nonstd::bad_any_cast &) {
                                return 0.0;
                            }
                        }
                    }
                }
            }
        }
    }
}

float DrVariant::toFloat() {
    try { return nonstd::any_cast<float>(m_value); }
    catch (const nonstd::bad_any_cast &) {
        try { return static_cast<float>(nonstd::any_cast<double>(m_value)); }
        catch (const nonstd::bad_any_cast &) {
            try { return static_cast<float>(nonstd::any_cast<int>(m_value)); }
            catch (const nonstd::bad_any_cast &) {
                try { return static_cast<float>(nonstd::any_cast<long>(m_value)); }
                catch (const nonstd::bad_any_cast &) {
                    try { return static_cast<float>(nonstd::any_cast<unsigned int>(m_value)); }
                    catch (const nonstd::bad_any_cast &) {
                        try { return static_cast<float>(nonstd::any_cast<unsigned long>(m_value)); }
                        catch (const nonstd::bad_any_cast &) {
                            try { return static_cast<float>(nonstd::any_cast<size_t>(m_value)); }
                            catch (const nonstd::bad_any_cast &) {
                                return 0.0f;
                            }
                        }
                    }
                }
            }
        }
    }
}

int DrVariant::toInt() {
    try { return nonstd::any_cast<int>(m_value); }
    catch (const nonstd::bad_any_cast &) {
        try { return static_cast<int>(nonstd::any_cast<long>(m_value)); }
        catch (const nonstd::bad_any_cast &) {
            try { return static_cast<int>(nonstd::any_cast<unsigned int>(m_value)); }
            catch (const nonstd::bad_any_cast &) {
                try { return static_cast<int>(nonstd::any_cast<unsigned long>(m_value)); }
                catch (const nonstd::bad_any_cast &) {
                    try { return static_cast<int>(nonstd::any_cast<double>(m_value)); }
                    catch (const nonstd::bad_any_cast &) {
                        try { return static_cast<int>(nonstd::any_cast<float>(m_value)); }
                        catch (const nonstd::bad_any_cast &) {
                            try { return static_cast<int>(nonstd::any_cast<size_t>(m_value)); }
                            catch (const nonstd::bad_any_cast &) {
                                return 0;
                            }
                        }
                    }
                }
            }
        }
    }
}

long DrVariant::toLong() {
    try { return nonstd::any_cast<long>(m_value); }
    catch (const nonstd::bad_any_cast &) {
        try { return static_cast<long>(nonstd::any_cast<int>(m_value)); }
        catch (const nonstd::bad_any_cast &) {
            try { return static_cast<long>(nonstd::any_cast<unsigned long>(m_value)); }
            catch (const nonstd::bad_any_cast &) {
                try { return static_cast<long>(nonstd::any_cast<unsigned int>(m_value)); }
                catch (const nonstd::bad_any_cast &) {
                    try { return static_cast<long>(nonstd::any_cast<double>(m_value)); }
                    catch (const nonstd::bad_any_cast &) {
                        try { return static_cast<long>(nonstd::any_cast<float>(m_value)); }
                        catch (const nonstd::bad_any_cast &) {
                            try { return static_cast<long>(nonstd::any_cast<size_t>(m_value)); }
                            catch (const nonstd::bad_any_cast &) {
                                return 0;
                            }
                        }
                    }
                }
            }
        }
    }
}

unsigned int DrVariant::toUInt() {
    try { return nonstd::any_cast<unsigned int>(m_value); }
    catch (const nonstd::bad_any_cast &) {
        try { return static_cast<unsigned int>(nonstd::any_cast<unsigned long>(m_value)); }
        catch (const nonstd::bad_any_cast &) {
            try { return static_cast<unsigned int>(nonstd::any_cast<int>(m_value)); }
            catch (const nonstd::bad_any_cast &) {
                try { return static_cast<unsigned int>(nonstd::any_cast<long>(m_value)); }
                catch (const nonstd::bad_any_cast &) {
                    try { return static_cast<unsigned int>(nonstd::any_cast<size_t>(m_value)); }
                    catch (const nonstd::bad_any_cast &) {
                        return 0;
                    }
                }
            }
        }
    }
}

//####################################################################################
//##    Std Data Type Returns
//####################################################################################
std::string DrVariant::toString() {

    // #NOTE: Alternate type casting check, however, wary of consistent type names across platforms
    ///if (m_value.type() == typeid(std::string)) {

    try { return nonstd::any_cast<std::string>(m_value); }
    catch (const nonstd::bad_any_cast &) {
        try { return std::string(nonstd::any_cast<const char*>(m_value)); }
        catch (const nonstd::bad_any_cast &) {
            return std::string("");
        }
    }
}

//####################################################################################
//##    Container Data Type Returns
//####################################################################################
// Try to return from Deque, or from Vector, otherwise return empty Vector
std::deque<DrVariant> DrVariant::toDeque() {
    try { return nonstd::any_cast<std::deque<DrVariant>>(m_value); }
    catch (const nonstd::bad_any_cast &) {
        try { std::vector<DrVariant> as_vector = nonstd::any_cast<std::vector<DrVariant>>(m_value);
              return { as_vector.begin(), as_vector.end() }; }
        catch (const nonstd::bad_any_cast &) { return std::deque<DrVariant>(); } }
}
std::list<DrVariant> DrVariant::toList() {
    try { return nonstd::any_cast<std::list<DrVariant>>(m_value); }
    catch (const nonstd::bad_any_cast &) {   return std::list<DrVariant>(); }
}
// Try to return from Vector, or from Deque, otherwise return empty Vector
std::vector<DrVariant> DrVariant::toVector() {
    try { return nonstd::any_cast<std::vector<DrVariant>>(m_value); }
    catch (const nonstd::bad_any_cast &) {
        try { std::deque<DrVariant> as_deque = nonstd::any_cast<std::deque<DrVariant>>(m_value);
              return { as_deque.begin(), as_deque.end() }; }
        catch (const nonstd::bad_any_cast &) { return std::vector<DrVariant>(); }
    }
}

//####################################################################################
//##    Custom Data Type Returns
//####################################################################################
// Try to return from DrColor, otherwise unsigned int/long, otherwise return DrColor(0, 0, 0, 0);
DrColor DrVariant::toColor() {
    try { return nonstd::any_cast<DrColor>(m_value); }
    catch (const nonstd::bad_any_cast &) {
        try { DrColor color(nonstd::any_cast<unsigned int>(m_value));
              return color; }
        catch (const nonstd::bad_any_cast &) {
            try { DrColor color(static_cast<unsigned int>(nonstd::any_cast<unsigned long>(m_value)));
                  return color; }
            catch (const nonstd::bad_any_cast &) {
                return DrColor(0, 0, 0, 0);
            }
        }
    }
}
// Try to return from DrImage
DrBitmap DrVariant::toImage() {
    try { return nonstd::any_cast<DrBitmap>(m_value); }
    catch (const nonstd::bad_any_cast &) {
        return DrBitmap();
    }
}
// Try to return from DrPoint, otherwise DrPointF, otherwise return Point(0, 0)
DrPoint DrVariant::toPoint() {
    try { return nonstd::any_cast<DrPoint>(m_value); }
    catch (const nonstd::bad_any_cast &) {
        try { DrPointF pointf = nonstd::any_cast<DrPointF>(m_value);
              return DrPoint(pointf.x, pointf.y); }
        catch (const nonstd::bad_any_cast &) {
            return DrPoint(0, 0);
        }
    }
}
// Try to return from DrPointF, otherwise DrPoint, otherwise return Point(0, 0)
DrPointF DrVariant::toPointF() {
    try { return nonstd::any_cast<DrPointF>(m_value); }
    catch (const nonstd::bad_any_cast &) {
        try { DrPoint point = nonstd::any_cast<DrPoint>(m_value);
              return DrPointF(point.x, point.y); }
        catch (const nonstd::bad_any_cast &) {
            return DrPointF(0, 0);
        }
    }
}
// Try to return from DrPolygonF
DrPolygonF DrVariant::toPolygonF() {
    try { return nonstd::any_cast<DrPolygonF>(m_value); }
    catch (const nonstd::bad_any_cast &) {
        return DrPolygonF();
    }
}
// Try to return from DrRect, otherwise DrRectF, otherwise hmm_vec4, otherwise return DrRect(0, 0, 0, 0)
DrRect DrVariant::toRect() {
    try { return nonstd::any_cast<DrRect>(m_value); }
    catch (const nonstd::bad_any_cast &) {
        try { return DrRect(nonstd::any_cast<DrRectF>(m_value)); }
        catch (const nonstd::bad_any_cast &) {
            try { return DrRect(nonstd::any_cast<hmm_vec4>(m_value)); }
            catch (const nonstd::bad_any_cast &) {
                return DrRect(0, 0, 0, 0);
            }
        }
    }
}
// Try to return from DrRectF, otherwise DrRect, otherwise hmm_vec4, otherwise return DrRectF(0, 0, 0, 0)
DrRectF DrVariant::toRectF() {
    try { return nonstd::any_cast<DrRectF>(m_value); }
    catch (const nonstd::bad_any_cast &) {
        try { return DrRectF(nonstd::any_cast<DrRect>(m_value)); }
        catch (const nonstd::bad_any_cast &) {
            try { return DrRectF(nonstd::any_cast<hmm_vec4>(m_value)); }
            catch (const nonstd::bad_any_cast &) {
                return DrRectF(0, 0, 0, 0);
            }
        }
    }
}
// Try to return from DrVec2, otherwise hmm_vec2, otherwise DrPointF/DrPoint, otherwise return Vec2(0, 0)
DrVec2 DrVariant::toVec2() {
    try { return nonstd::any_cast<DrVec2>(m_value); }
    catch (const nonstd::bad_any_cast &) {
        try { hmm_vec2 vec2 = nonstd::any_cast<hmm_vec2>(m_value);
              return DrVec2(vec2); }
        catch (const nonstd::bad_any_cast &) {
            try { DrPointF pointf = nonstd::any_cast<DrPointF>(m_value);
                  return DrVec2(static_cast<float>(pointf.x), static_cast<float>(pointf.y)); }
            catch (const nonstd::bad_any_cast &) {
                try { DrPoint point = nonstd::any_cast<DrPoint>(m_value);
                      return DrVec2(static_cast<float>(point.x), static_cast<float>(point.y)); }
                catch (const nonstd::bad_any_cast &) {
                    return DrVec2(0, 0);
                }
            }
        }
    }
}
// Try to return from DrVec3, otherwise hmm_vec3, otherwise return Vec3(0, 0, 0)
DrVec3 DrVariant::toVec3() {
    try { return nonstd::any_cast<DrVec3>(m_value); }
    catch (const nonstd::bad_any_cast &) {
        try { hmm_vec3 vec3 = nonstd::any_cast<hmm_vec3>(m_value);
              return DrVec3(vec3); }
        catch (const nonstd::bad_any_cast &) {
            try {   std::vector<DrVariant> vector = nonstd::any_cast<std::vector<DrVariant>>(m_value);
                    DrVec3 vec3;
                    if (vector.size() >= 1) vec3.setX(vector[0].toDouble());
                    if (vector.size() >= 2) vec3.setY(vector[1].toDouble());
                    if (vector.size() >= 3) vec3.setZ(vector[2].toDouble());
                    return vec3; }
            catch (const nonstd::bad_any_cast &) {
                return DrVec3(0, 0, 0);
            }
        }
    }
}










