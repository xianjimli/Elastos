
#ifndef __CONTENT_H__
#define __CONTENT_H__

#include <string>
#include "Lists.h"

class Content {
public:
    // constructions
    Content();
    Content(std::string text, int order, int level, std::string href);

    // set Content values functions
    void setText(std::string text);
    void setOrder(int order);
    void setLevel(int level);
    void setHRef(std::string href);

    // get Content values functions
    std::string getText() const;
    int getOrder() const;
    int getLevel() const;
    std::string getHRef() const;

private:
    std::string Text;
    int Order;
    int Level;
    std::string HRef;
};

inline void Content::setText(std::string text) { Text = text;; }
inline void Content::setOrder(int order) { Order = order; }
inline void Content::setLevel(int level) { Level = level; }
inline void Content::setHRef(std::string href) { HRef = href; }

inline std::string Content::getText() const { return Text; }
inline int Content::getOrder() const { return Order; }
inline int Content::getLevel() const { return Level; }
inline std::string Content::getHRef() const { return HRef; }

#endif /* __CONTENT_H__ */
