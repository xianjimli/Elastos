
#include "Content.h"

Content::Content(): Text(""), Order(0), Level(0), HRef(""){
}

Content::Content(std::string text, int order, int level, std::string href): Text(text), Order(order), Level(level), HRef(href) {
}
