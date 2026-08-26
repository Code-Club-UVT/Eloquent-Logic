//
// Created by vscode on 7/30/26.
//

#ifndef ELOQUENTLOGIC_FEEDBACK_LISTENER_HPP
#define ELOQUENTLOGIC_FEEDBACK_LISTENER_HPP
#define CB_FULL(n, p)                                                          \
    virtual void n p {}
#define CB(n) CB_FULL(n, (const lexeme &))
#define CB_N(n) CB_FULL(n, ())
namespace eloquent::logic {
class feedback_listener {
  public:
    virtual ~feedback_listener() = default;
    virtual void didStart() {}
    virtual void didFinish() {}
};
} // namespace eloquent::logic
#endif // ELOQUENTLOGIC_FEEDBACK_LISTENER_HPP
