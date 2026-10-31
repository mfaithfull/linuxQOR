# Product Context

The QOR addresses the neglected `utilities`/`utils` layer in application source trees: code that is critical to an application but is neither application logic nor framework logic. The project re-examines application-over-framework layering through two ideas:

- Separation of Concerns: portability, adaptability, and reduced coupling to dependencies.
- Self Aware Software: make the software do to itself what a computer can do well - test, build, report, scale, manage memory, port, connect, and examine itself.

Problems it solves for users:
- Write an application once, run it everywhere and forever at full speed.
- Keep all source readable, understandable, and changeable.
- Be ready for new OS, hardware, and toolchains with existing code.
- Use advanced C++ techniques without rewriting them: reflection, smart references, DI, pipelines, interception, async IO, thread pools, signals/slots.
- Choose object lifetime and memory-management strategy deliberately.
- Replace selected parts without duplicating large source trees.

Intended usage flow:
1. Learn the organization.
2. Configure the QOR.
3. Build it for the chosen OS/compiler/toolchain with CMake.
4. Write an application against `sdk/` headers.
5. Link against the built QOR runtime.

User experience goals: almost everything is optional and overridable; modules work together out of the box; adding modules is short work; TDD tools are integrated; data pipelines, workflows, properties, signals, delegates, and policy-based memory management are available.

Current product outcome: the project is working toward a TUI/GUI scene-template system. Windows GDI, Wayland, X, layout, terminal, JSON, and YAML groundwork exist. The JSON generator write path and parse/generate/parse roundtrip are complete and green. A full assembly layer is still required.

Contributions and starter work are listed in `CONTRIBUTING.md`.
