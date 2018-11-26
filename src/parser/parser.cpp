// A Bison parser, made by GNU Bison 3.0.4.

// Skeleton implementation for Bison LALR(1) parsers in C++

// Copyright (C) 2002-2015 Free Software Foundation, Inc.

// This program is free software: you can redistribute it and/or modify
// it under the terms of the GNU General Public License as published by
// the Free Software Foundation, either version 3 of the License, or
// (at your option) any later version.

// This program is distributed in the hope that it will be useful,
// but WITHOUT ANY WARRANTY; without even the implied warranty of
// MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
// GNU General Public License for more details.

// You should have received a copy of the GNU General Public License
// along with this program.  If not, see <http://www.gnu.org/licenses/>.

// As a special exception, you may create a larger work that contains
// part or all of the Bison parser skeleton and distribute that work
// under terms of your choice, so long as that work isn't itself a
// parser generator using the skeleton or a modified version thereof
// as a parser skeleton.  Alternatively, if you modify or redistribute
// the parser skeleton itself, you may (at your option) remove this
// special exception, which will cause the skeleton and the resulting
// Bison output files to be licensed under the GNU General Public
// License without this special exception.

// This special exception was added by the Free Software Foundation in
// version 2.2 of Bison.


// First part of user declarations.

#line 37 "/home/jiawei/Dropbox/Graduate/COMPILERS/repo/src/parser/parser.cpp" // lalr1.cc:404

# ifndef YY_NULLPTR
#  if defined __cplusplus && 201103L <= __cplusplus
#   define YY_NULLPTR nullptr
#  else
#   define YY_NULLPTR 0
#  endif
# endif

#include "parser.hpp"

// User implementation prologue.

#line 51 "/home/jiawei/Dropbox/Graduate/COMPILERS/repo/src/parser/parser.cpp" // lalr1.cc:412
// Unqualified %code blocks.
#line 21 "/home/jiawei/Dropbox/Graduate/COMPILERS/repo/src/parser/parser.y" // lalr1.cc:413

	#include "node.h"
	Prog *rootProg;

	static int yylex(bison::Parser::semantic_type *yylval,
	Lexer &lexer);

#line 61 "/home/jiawei/Dropbox/Graduate/COMPILERS/repo/src/parser/parser.cpp" // lalr1.cc:413


#ifndef YY_
# if defined YYENABLE_NLS && YYENABLE_NLS
#  if ENABLE_NLS
#   include <libintl.h> // FIXME: INFRINGES ON USER NAME SPACE.
#   define YY_(msgid) dgettext ("bison-runtime", msgid)
#  endif
# endif
# ifndef YY_
#  define YY_(msgid) msgid
# endif
#endif



// Suppress unused-variable warnings by "using" E.
#define YYUSE(E) ((void) (E))

// Enable debugging if requested.
#if YYDEBUG

// A pseudo ostream that takes yydebug_ into account.
# define YYCDEBUG if (yydebug_) (*yycdebug_)

# define YY_SYMBOL_PRINT(Title, Symbol)         \
  do {                                          \
    if (yydebug_)                               \
    {                                           \
      *yycdebug_ << Title << ' ';               \
      yy_print_ (*yycdebug_, Symbol);           \
      *yycdebug_ << std::endl;                  \
    }                                           \
  } while (false)

# define YY_REDUCE_PRINT(Rule)          \
  do {                                  \
    if (yydebug_)                       \
      yy_reduce_print_ (Rule);          \
  } while (false)

# define YY_STACK_PRINT()               \
  do {                                  \
    if (yydebug_)                       \
      yystack_print_ ();                \
  } while (false)

#else // !YYDEBUG

# define YYCDEBUG if (false) std::cerr
# define YY_SYMBOL_PRINT(Title, Symbol)  YYUSE(Symbol)
# define YY_REDUCE_PRINT(Rule)           static_cast<void>(0)
# define YY_STACK_PRINT()                static_cast<void>(0)

#endif // !YYDEBUG

#define yyerrok         (yyerrstatus_ = 0)
#define yyclearin       (yyla.clear ())

#define YYACCEPT        goto yyacceptlab
#define YYABORT         goto yyabortlab
#define YYERROR         goto yyerrorlab
#define YYRECOVERING()  (!!yyerrstatus_)

#line 7 "/home/jiawei/Dropbox/Graduate/COMPILERS/repo/src/parser/parser.y" // lalr1.cc:479
namespace bison {
#line 128 "/home/jiawei/Dropbox/Graduate/COMPILERS/repo/src/parser/parser.cpp" // lalr1.cc:479

  /* Return YYSTR after stripping away unnecessary quotes and
     backslashes, so that it's suitable for yyerror.  The heuristic is
     that double-quoting is unnecessary unless the string contains an
     apostrophe, a comma, or backslash (other than backslash-backslash).
     YYSTR is taken from yytname.  */
  std::string
  Parser::yytnamerr_ (const char *yystr)
  {
    if (*yystr == '"')
      {
        std::string yyr = "";
        char const *yyp = yystr;

        for (;;)
          switch (*++yyp)
            {
            case '\'':
            case ',':
              goto do_not_strip_quotes;

            case '\\':
              if (*++yyp != '\\')
                goto do_not_strip_quotes;
              // Fall through.
            default:
              yyr += *yyp;
              break;

            case '"':
              return yyr;
            }
      do_not_strip_quotes: ;
      }

    return yystr;
  }


  /// Build a parser object.
  Parser::Parser (Lexer &lexer_yyarg, Prog &prog_yyarg)
    :
#if YYDEBUG
      yydebug_ (false),
      yycdebug_ (&std::cerr),
#endif
      lexer (lexer_yyarg),
      prog (prog_yyarg)
  {}

  Parser::~Parser ()
  {}


  /*---------------.
  | Symbol types.  |
  `---------------*/

  inline
  Parser::syntax_error::syntax_error (const std::string& m)
    : std::runtime_error (m)
  {}

  // basic_symbol.
  template <typename Base>
  inline
  Parser::basic_symbol<Base>::basic_symbol ()
    : value ()
  {}

  template <typename Base>
  inline
  Parser::basic_symbol<Base>::basic_symbol (const basic_symbol& other)
    : Base (other)
    , value ()
  {
    value = other.value;
  }


  template <typename Base>
  inline
  Parser::basic_symbol<Base>::basic_symbol (typename Base::kind_type t, const semantic_type& v)
    : Base (t)
    , value (v)
  {}


  /// Constructor for valueless symbols.
  template <typename Base>
  inline
  Parser::basic_symbol<Base>::basic_symbol (typename Base::kind_type t)
    : Base (t)
    , value ()
  {}

  template <typename Base>
  inline
  Parser::basic_symbol<Base>::~basic_symbol ()
  {
    clear ();
  }

  template <typename Base>
  inline
  void
  Parser::basic_symbol<Base>::clear ()
  {
    Base::clear ();
  }

  template <typename Base>
  inline
  bool
  Parser::basic_symbol<Base>::empty () const
  {
    return Base::type_get () == empty_symbol;
  }

  template <typename Base>
  inline
  void
  Parser::basic_symbol<Base>::move (basic_symbol& s)
  {
    super_type::move(s);
    value = s.value;
  }

  // by_type.
  inline
  Parser::by_type::by_type ()
    : type (empty_symbol)
  {}

  inline
  Parser::by_type::by_type (const by_type& other)
    : type (other.type)
  {}

  inline
  Parser::by_type::by_type (token_type t)
    : type (yytranslate_ (t))
  {}

  inline
  void
  Parser::by_type::clear ()
  {
    type = empty_symbol;
  }

  inline
  void
  Parser::by_type::move (by_type& that)
  {
    type = that.type;
    that.clear ();
  }

  inline
  int
  Parser::by_type::type_get () const
  {
    return type;
  }


  // by_state.
  inline
  Parser::by_state::by_state ()
    : state (empty_state)
  {}

  inline
  Parser::by_state::by_state (const by_state& other)
    : state (other.state)
  {}

  inline
  void
  Parser::by_state::clear ()
  {
    state = empty_state;
  }

  inline
  void
  Parser::by_state::move (by_state& that)
  {
    state = that.state;
    that.clear ();
  }

  inline
  Parser::by_state::by_state (state_type s)
    : state (s)
  {}

  inline
  Parser::symbol_number_type
  Parser::by_state::type_get () const
  {
    if (state == empty_state)
      return empty_symbol;
    else
      return yystos_[state];
  }

  inline
  Parser::stack_symbol_type::stack_symbol_type ()
  {}


  inline
  Parser::stack_symbol_type::stack_symbol_type (state_type s, symbol_type& that)
    : super_type (s)
  {
    value = that.value;
    // that is emptied.
    that.type = empty_symbol;
  }

  inline
  Parser::stack_symbol_type&
  Parser::stack_symbol_type::operator= (const stack_symbol_type& that)
  {
    state = that.state;
    value = that.value;
    return *this;
  }


  template <typename Base>
  inline
  void
  Parser::yy_destroy_ (const char* yymsg, basic_symbol<Base>& yysym) const
  {
    if (yymsg)
      YY_SYMBOL_PRINT (yymsg, yysym);

    // User destructor.
    YYUSE (yysym.type_get ());
  }

#if YYDEBUG
  template <typename Base>
  void
  Parser::yy_print_ (std::ostream& yyo,
                                     const basic_symbol<Base>& yysym) const
  {
    std::ostream& yyoutput = yyo;
    YYUSE (yyoutput);
    symbol_number_type yytype = yysym.type_get ();
    // Avoid a (spurious) G++ 4.8 warning about "array subscript is
    // below array bounds".
    if (yysym.empty ())
      std::abort ();
    yyo << (yytype < yyntokens_ ? "token" : "nterm")
        << ' ' << yytname_[yytype] << " (";
    YYUSE (yytype);
    yyo << ')';
  }
#endif

  inline
  void
  Parser::yypush_ (const char* m, state_type s, symbol_type& sym)
  {
    stack_symbol_type t (s, sym);
    yypush_ (m, t);
  }

  inline
  void
  Parser::yypush_ (const char* m, stack_symbol_type& s)
  {
    if (m)
      YY_SYMBOL_PRINT (m, s);
    yystack_.push (s);
  }

  inline
  void
  Parser::yypop_ (unsigned int n)
  {
    yystack_.pop (n);
  }

#if YYDEBUG
  std::ostream&
  Parser::debug_stream () const
  {
    return *yycdebug_;
  }

  void
  Parser::set_debug_stream (std::ostream& o)
  {
    yycdebug_ = &o;
  }


  Parser::debug_level_type
  Parser::debug_level () const
  {
    return yydebug_;
  }

  void
  Parser::set_debug_level (debug_level_type l)
  {
    yydebug_ = l;
  }
#endif // YYDEBUG

  inline Parser::state_type
  Parser::yy_lr_goto_state_ (state_type yystate, int yysym)
  {
    int yyr = yypgoto_[yysym - yyntokens_] + yystate;
    if (0 <= yyr && yyr <= yylast_ && yycheck_[yyr] == yystate)
      return yytable_[yyr];
    else
      return yydefgoto_[yysym - yyntokens_];
  }

  inline bool
  Parser::yy_pact_value_is_default_ (int yyvalue)
  {
    return yyvalue == yypact_ninf_;
  }

  inline bool
  Parser::yy_table_value_is_error_ (int yyvalue)
  {
    return yyvalue == yytable_ninf_;
  }

  int
  Parser::parse ()
  {
    // State.
    int yyn;
    /// Length of the RHS of the rule being reduced.
    int yylen = 0;

    // Error handling.
    int yynerrs_ = 0;
    int yyerrstatus_ = 0;

    /// The lookahead symbol.
    symbol_type yyla;

    /// The return value of parse ().
    int yyresult;

    // FIXME: This shoud be completely indented.  It is not yet to
    // avoid gratuitous conflicts when merging into the master branch.
    try
      {
    YYCDEBUG << "Starting parse" << std::endl;


    /* Initialize the stack.  The initial state will be set in
       yynewstate, since the latter expects the semantical and the
       location values to have been already stored, initialize these
       stacks with a primary value.  */
    yystack_.clear ();
    yypush_ (YY_NULLPTR, 0, yyla);

    // A new symbol was pushed on the stack.
  yynewstate:
    YYCDEBUG << "Entering state " << yystack_[0].state << std::endl;

    // Accept?
    if (yystack_[0].state == yyfinal_)
      goto yyacceptlab;

    goto yybackup;

    // Backup.
  yybackup:

    // Try to take a decision without lookahead.
    yyn = yypact_[yystack_[0].state];
    if (yy_pact_value_is_default_ (yyn))
      goto yydefault;

    // Read a lookahead token.
    if (yyla.empty ())
      {
        YYCDEBUG << "Reading a token: ";
        try
          {
            yyla.type = yytranslate_ (yylex (&yyla.value, lexer));
          }
        catch (const syntax_error& yyexc)
          {
            error (yyexc);
            goto yyerrlab1;
          }
      }
    YY_SYMBOL_PRINT ("Next token is", yyla);

    /* If the proper action on seeing token YYLA.TYPE is to reduce or
       to detect an error, take that action.  */
    yyn += yyla.type_get ();
    if (yyn < 0 || yylast_ < yyn || yycheck_[yyn] != yyla.type_get ())
      goto yydefault;

    // Reduce or error.
    yyn = yytable_[yyn];
    if (yyn <= 0)
      {
        if (yy_table_value_is_error_ (yyn))
          goto yyerrlab;
        yyn = -yyn;
        goto yyreduce;
      }

    // Count tokens shifted since error; after three, turn off error status.
    if (yyerrstatus_)
      --yyerrstatus_;

    // Shift the lookahead token.
    yypush_ ("Shifting", yyn, yyla);
    goto yynewstate;

  /*-----------------------------------------------------------.
  | yydefault -- do the default action for the current state.  |
  `-----------------------------------------------------------*/
  yydefault:
    yyn = yydefact_[yystack_[0].state];
    if (yyn == 0)
      goto yyerrlab;
    goto yyreduce;

  /*-----------------------------.
  | yyreduce -- Do a reduction.  |
  `-----------------------------*/
  yyreduce:
    yylen = yyr2_[yyn];
    {
      stack_symbol_type yylhs;
      yylhs.state = yy_lr_goto_state_(yystack_[yylen].state, yyr1_[yyn]);
      /* If YYLEN is nonzero, implement the default value of the
         action: '$$ = $1'.  Otherwise, use the top of the stack.

         Otherwise, the following line sets YYLHS.VALUE to garbage.
         This behavior is undocumented and Bison users should not rely
         upon it.  */
      if (yylen)
        yylhs.value = yystack_[yylen - 1].value;
      else
        yylhs.value = yystack_[0].value;


      // Perform the reduction.
      YY_REDUCE_PRINT (yyn);
      try
        {
          switch (yyn)
            {
  case 2:
#line 117 "/home/jiawei/Dropbox/Graduate/COMPILERS/repo/src/parser/parser.y" // lalr1.cc:859
    {(yylhs.value.progType) = new Prog((yystack_[1].value.externsType), (yystack_[0].value.funcsType)); rootProg = (yylhs.value.progType);}
#line 594 "/home/jiawei/Dropbox/Graduate/COMPILERS/repo/src/parser/parser.cpp" // lalr1.cc:859
    break;

  case 3:
#line 118 "/home/jiawei/Dropbox/Graduate/COMPILERS/repo/src/parser/parser.y" // lalr1.cc:859
    {(yylhs.value.progType) = new Prog(nullptr, (yystack_[0].value.funcsType)); rootProg = (yylhs.value.progType);}
#line 600 "/home/jiawei/Dropbox/Graduate/COMPILERS/repo/src/parser/parser.cpp" // lalr1.cc:859
    break;

  case 4:
#line 119 "/home/jiawei/Dropbox/Graduate/COMPILERS/repo/src/parser/parser.y" // lalr1.cc:859
    {/* empty file */}
#line 606 "/home/jiawei/Dropbox/Graduate/COMPILERS/repo/src/parser/parser.cpp" // lalr1.cc:859
    break;

  case 5:
#line 122 "/home/jiawei/Dropbox/Graduate/COMPILERS/repo/src/parser/parser.y" // lalr1.cc:859
    {(yystack_[1].value.externsType)->push_back((yystack_[0].value.externType)); (yylhs.value.externsType) = (yystack_[1].value.externsType);}
#line 612 "/home/jiawei/Dropbox/Graduate/COMPILERS/repo/src/parser/parser.cpp" // lalr1.cc:859
    break;

  case 6:
#line 123 "/home/jiawei/Dropbox/Graduate/COMPILERS/repo/src/parser/parser.y" // lalr1.cc:859
    {(yylhs.value.externsType) = new ExternList(); (yylhs.value.externsType)->push_back((yystack_[0].value.externType));}
#line 618 "/home/jiawei/Dropbox/Graduate/COMPILERS/repo/src/parser/parser.cpp" // lalr1.cc:859
    break;

  case 7:
#line 127 "/home/jiawei/Dropbox/Graduate/COMPILERS/repo/src/parser/parser.y" // lalr1.cc:859
    {(yylhs.value.externType) = new Extern((yystack_[5].value.tdeclType), (yystack_[4].value.globidType), (yystack_[2].value.tdeclsType));}
#line 624 "/home/jiawei/Dropbox/Graduate/COMPILERS/repo/src/parser/parser.cpp" // lalr1.cc:859
    break;

  case 8:
#line 128 "/home/jiawei/Dropbox/Graduate/COMPILERS/repo/src/parser/parser.y" // lalr1.cc:859
    {(yylhs.value.externType) = new Extern((yystack_[4].value.tdeclType), (yystack_[3].value.globidType));}
#line 630 "/home/jiawei/Dropbox/Graduate/COMPILERS/repo/src/parser/parser.cpp" // lalr1.cc:859
    break;

  case 9:
#line 131 "/home/jiawei/Dropbox/Graduate/COMPILERS/repo/src/parser/parser.y" // lalr1.cc:859
    {(yystack_[1].value.funcsType)->push_back((yystack_[0].value.funcdeclType)); (yylhs.value.funcsType) = (yystack_[1].value.funcsType);}
#line 636 "/home/jiawei/Dropbox/Graduate/COMPILERS/repo/src/parser/parser.cpp" // lalr1.cc:859
    break;

  case 10:
#line 132 "/home/jiawei/Dropbox/Graduate/COMPILERS/repo/src/parser/parser.y" // lalr1.cc:859
    {(yylhs.value.funcsType) = new FuncList(); (yylhs.value.funcsType)->push_back((yystack_[0].value.funcdeclType));}
#line 642 "/home/jiawei/Dropbox/Graduate/COMPILERS/repo/src/parser/parser.cpp" // lalr1.cc:859
    break;

  case 11:
#line 135 "/home/jiawei/Dropbox/Graduate/COMPILERS/repo/src/parser/parser.y" // lalr1.cc:859
    {(yylhs.value.funcdeclType) = new FuncDecl((yystack_[5].value.tdeclType), (yystack_[4].value.globidType), (yystack_[0].value.blkType), (yystack_[2].value.vdeclsType));}
#line 648 "/home/jiawei/Dropbox/Graduate/COMPILERS/repo/src/parser/parser.cpp" // lalr1.cc:859
    break;

  case 12:
#line 136 "/home/jiawei/Dropbox/Graduate/COMPILERS/repo/src/parser/parser.y" // lalr1.cc:859
    {(yylhs.value.funcdeclType) = new FuncDecl((yystack_[4].value.tdeclType), (yystack_[3].value.globidType), (yystack_[0].value.blkType));}
#line 654 "/home/jiawei/Dropbox/Graduate/COMPILERS/repo/src/parser/parser.cpp" // lalr1.cc:859
    break;

  case 13:
#line 139 "/home/jiawei/Dropbox/Graduate/COMPILERS/repo/src/parser/parser.y" // lalr1.cc:859
    {(yylhs.value.blkType) = new Blk(*(yystack_[1].value.blkType));}
#line 660 "/home/jiawei/Dropbox/Graduate/COMPILERS/repo/src/parser/parser.cpp" // lalr1.cc:859
    break;

  case 14:
#line 140 "/home/jiawei/Dropbox/Graduate/COMPILERS/repo/src/parser/parser.y" // lalr1.cc:859
    {(yylhs.value.blkType) = new Blk();}
#line 666 "/home/jiawei/Dropbox/Graduate/COMPILERS/repo/src/parser/parser.cpp" // lalr1.cc:859
    break;

  case 15:
#line 143 "/home/jiawei/Dropbox/Graduate/COMPILERS/repo/src/parser/parser.y" // lalr1.cc:859
    {(yystack_[1].value.blkType)->statements.push_back((yystack_[0].value.stmtType)); (yylhs.value.blkType) = (yystack_[1].value.blkType);}
#line 672 "/home/jiawei/Dropbox/Graduate/COMPILERS/repo/src/parser/parser.cpp" // lalr1.cc:859
    break;

  case 16:
#line 144 "/home/jiawei/Dropbox/Graduate/COMPILERS/repo/src/parser/parser.y" // lalr1.cc:859
    {(yylhs.value.blkType) = new Blk(); (yylhs.value.blkType)->statements.push_back((yystack_[0].value.stmtType));}
#line 678 "/home/jiawei/Dropbox/Graduate/COMPILERS/repo/src/parser/parser.cpp" // lalr1.cc:859
    break;

  case 17:
#line 147 "/home/jiawei/Dropbox/Graduate/COMPILERS/repo/src/parser/parser.y" // lalr1.cc:859
    {(yylhs.value.stmtType) = (yystack_[0].value.blkType);}
#line 684 "/home/jiawei/Dropbox/Graduate/COMPILERS/repo/src/parser/parser.cpp" // lalr1.cc:859
    break;

  case 18:
#line 148 "/home/jiawei/Dropbox/Graduate/COMPILERS/repo/src/parser/parser.y" // lalr1.cc:859
    {(yylhs.value.stmtType) = new ReturnStmt((yystack_[1].value.expType));}
#line 690 "/home/jiawei/Dropbox/Graduate/COMPILERS/repo/src/parser/parser.cpp" // lalr1.cc:859
    break;

  case 19:
#line 149 "/home/jiawei/Dropbox/Graduate/COMPILERS/repo/src/parser/parser.y" // lalr1.cc:859
    {(yylhs.value.stmtType) = new ReturnStmt();}
#line 696 "/home/jiawei/Dropbox/Graduate/COMPILERS/repo/src/parser/parser.cpp" // lalr1.cc:859
    break;

  case 20:
#line 150 "/home/jiawei/Dropbox/Graduate/COMPILERS/repo/src/parser/parser.y" // lalr1.cc:859
    {(yylhs.value.stmtType) = new AssignStmt((yystack_[3].value.vdeclType), (yystack_[1].value.expType));}
#line 702 "/home/jiawei/Dropbox/Graduate/COMPILERS/repo/src/parser/parser.cpp" // lalr1.cc:859
    break;

  case 21:
#line 151 "/home/jiawei/Dropbox/Graduate/COMPILERS/repo/src/parser/parser.y" // lalr1.cc:859
    {(yylhs.value.stmtType) = new ExpStmt((yystack_[1].value.expType));}
#line 708 "/home/jiawei/Dropbox/Graduate/COMPILERS/repo/src/parser/parser.cpp" // lalr1.cc:859
    break;

  case 22:
#line 152 "/home/jiawei/Dropbox/Graduate/COMPILERS/repo/src/parser/parser.y" // lalr1.cc:859
    {(yylhs.value.stmtType) = new WhileStmt((yystack_[2].value.expType), (yystack_[0].value.stmtType));}
#line 714 "/home/jiawei/Dropbox/Graduate/COMPILERS/repo/src/parser/parser.cpp" // lalr1.cc:859
    break;

  case 23:
#line 153 "/home/jiawei/Dropbox/Graduate/COMPILERS/repo/src/parser/parser.y" // lalr1.cc:859
    {(yylhs.value.stmtType) = new IfStmt((yystack_[4].value.expType), (yystack_[2].value.stmtType), (yystack_[0].value.stmtType));}
#line 720 "/home/jiawei/Dropbox/Graduate/COMPILERS/repo/src/parser/parser.cpp" // lalr1.cc:859
    break;

  case 24:
#line 154 "/home/jiawei/Dropbox/Graduate/COMPILERS/repo/src/parser/parser.y" // lalr1.cc:859
    {(yylhs.value.stmtType) = new IfStmt((yystack_[2].value.expType), (yystack_[0].value.stmtType));}
#line 726 "/home/jiawei/Dropbox/Graduate/COMPILERS/repo/src/parser/parser.cpp" // lalr1.cc:859
    break;

  case 25:
#line 155 "/home/jiawei/Dropbox/Graduate/COMPILERS/repo/src/parser/parser.y" // lalr1.cc:859
    {(yylhs.value.stmtType) = new PrintStmt((yystack_[1].value.expType));}
#line 732 "/home/jiawei/Dropbox/Graduate/COMPILERS/repo/src/parser/parser.cpp" // lalr1.cc:859
    break;

  case 26:
#line 156 "/home/jiawei/Dropbox/Graduate/COMPILERS/repo/src/parser/parser.y" // lalr1.cc:859
    {(yylhs.value.stmtType) = new PrintSlitStmt((yystack_[1].value.strType));}
#line 738 "/home/jiawei/Dropbox/Graduate/COMPILERS/repo/src/parser/parser.cpp" // lalr1.cc:859
    break;

  case 27:
#line 159 "/home/jiawei/Dropbox/Graduate/COMPILERS/repo/src/parser/parser.y" // lalr1.cc:859
    {(yystack_[2].value.expsType)->push_back((yystack_[0].value.expType)); (yylhs.value.expsType) = (yystack_[2].value.expsType);}
#line 744 "/home/jiawei/Dropbox/Graduate/COMPILERS/repo/src/parser/parser.cpp" // lalr1.cc:859
    break;

  case 28:
#line 160 "/home/jiawei/Dropbox/Graduate/COMPILERS/repo/src/parser/parser.y" // lalr1.cc:859
    {(yylhs.value.expsType) = new ExpList(); (yylhs.value.expsType)->push_back((yystack_[0].value.expType));}
#line 750 "/home/jiawei/Dropbox/Graduate/COMPILERS/repo/src/parser/parser.cpp" // lalr1.cc:859
    break;

  case 29:
#line 163 "/home/jiawei/Dropbox/Graduate/COMPILERS/repo/src/parser/parser.y" // lalr1.cc:859
    {(yylhs.value.expType) = (yystack_[1].value.expType);}
#line 756 "/home/jiawei/Dropbox/Graduate/COMPILERS/repo/src/parser/parser.cpp" // lalr1.cc:859
    break;

  case 30:
#line 164 "/home/jiawei/Dropbox/Graduate/COMPILERS/repo/src/parser/parser.y" // lalr1.cc:859
    {(yylhs.value.expType) = (yystack_[0].value.expType);}
#line 762 "/home/jiawei/Dropbox/Graduate/COMPILERS/repo/src/parser/parser.cpp" // lalr1.cc:859
    break;

  case 31:
#line 165 "/home/jiawei/Dropbox/Graduate/COMPILERS/repo/src/parser/parser.y" // lalr1.cc:859
    {(yylhs.value.expType) = (yystack_[0].value.expType);}
#line 768 "/home/jiawei/Dropbox/Graduate/COMPILERS/repo/src/parser/parser.cpp" // lalr1.cc:859
    break;

  case 32:
#line 166 "/home/jiawei/Dropbox/Graduate/COMPILERS/repo/src/parser/parser.y" // lalr1.cc:859
    {(yylhs.value.expType) = (yystack_[0].value.expType);}
#line 774 "/home/jiawei/Dropbox/Graduate/COMPILERS/repo/src/parser/parser.cpp" // lalr1.cc:859
    break;

  case 33:
#line 167 "/home/jiawei/Dropbox/Graduate/COMPILERS/repo/src/parser/parser.y" // lalr1.cc:859
    {(yylhs.value.expType) = (yystack_[0].value.varType);}
#line 780 "/home/jiawei/Dropbox/Graduate/COMPILERS/repo/src/parser/parser.cpp" // lalr1.cc:859
    break;

  case 34:
#line 168 "/home/jiawei/Dropbox/Graduate/COMPILERS/repo/src/parser/parser.y" // lalr1.cc:859
    {(yylhs.value.expType) = new FuncCall((yystack_[3].value.globidType), (yystack_[1].value.expsType));}
#line 786 "/home/jiawei/Dropbox/Graduate/COMPILERS/repo/src/parser/parser.cpp" // lalr1.cc:859
    break;

  case 35:
#line 169 "/home/jiawei/Dropbox/Graduate/COMPILERS/repo/src/parser/parser.y" // lalr1.cc:859
    {(yylhs.value.expType) = new FuncCall((yystack_[2].value.globidType));}
#line 792 "/home/jiawei/Dropbox/Graduate/COMPILERS/repo/src/parser/parser.cpp" // lalr1.cc:859
    break;

  case 36:
#line 180 "/home/jiawei/Dropbox/Graduate/COMPILERS/repo/src/parser/parser.y" // lalr1.cc:859
    {(yylhs.value.expType) = new BinOp((yystack_[2].value.expType), "=", (yystack_[0].value.expType));}
#line 798 "/home/jiawei/Dropbox/Graduate/COMPILERS/repo/src/parser/parser.cpp" // lalr1.cc:859
    break;

  case 37:
#line 181 "/home/jiawei/Dropbox/Graduate/COMPILERS/repo/src/parser/parser.y" // lalr1.cc:859
    {(yylhs.value.expType) = new BinOp((yystack_[2].value.expType), "||", (yystack_[0].value.expType));}
#line 804 "/home/jiawei/Dropbox/Graduate/COMPILERS/repo/src/parser/parser.cpp" // lalr1.cc:859
    break;

  case 38:
#line 182 "/home/jiawei/Dropbox/Graduate/COMPILERS/repo/src/parser/parser.y" // lalr1.cc:859
    {(yylhs.value.expType) = new BinOp((yystack_[2].value.expType), "&&", (yystack_[0].value.expType));}
#line 810 "/home/jiawei/Dropbox/Graduate/COMPILERS/repo/src/parser/parser.cpp" // lalr1.cc:859
    break;

  case 39:
#line 183 "/home/jiawei/Dropbox/Graduate/COMPILERS/repo/src/parser/parser.y" // lalr1.cc:859
    {(yylhs.value.expType) = new BinOp((yystack_[2].value.expType), "==", (yystack_[0].value.expType));}
#line 816 "/home/jiawei/Dropbox/Graduate/COMPILERS/repo/src/parser/parser.cpp" // lalr1.cc:859
    break;

  case 40:
#line 184 "/home/jiawei/Dropbox/Graduate/COMPILERS/repo/src/parser/parser.y" // lalr1.cc:859
    {(yylhs.value.expType) = new BinOp((yystack_[2].value.expType), ">", (yystack_[0].value.expType));}
#line 822 "/home/jiawei/Dropbox/Graduate/COMPILERS/repo/src/parser/parser.cpp" // lalr1.cc:859
    break;

  case 41:
#line 185 "/home/jiawei/Dropbox/Graduate/COMPILERS/repo/src/parser/parser.y" // lalr1.cc:859
    {(yylhs.value.expType) = new BinOp((yystack_[2].value.expType), "<", (yystack_[0].value.expType));}
#line 828 "/home/jiawei/Dropbox/Graduate/COMPILERS/repo/src/parser/parser.cpp" // lalr1.cc:859
    break;

  case 42:
#line 186 "/home/jiawei/Dropbox/Graduate/COMPILERS/repo/src/parser/parser.y" // lalr1.cc:859
    {(yylhs.value.expType) = new BinOp((yystack_[2].value.expType), "+", (yystack_[0].value.expType));}
#line 834 "/home/jiawei/Dropbox/Graduate/COMPILERS/repo/src/parser/parser.cpp" // lalr1.cc:859
    break;

  case 43:
#line 187 "/home/jiawei/Dropbox/Graduate/COMPILERS/repo/src/parser/parser.y" // lalr1.cc:859
    {(yylhs.value.expType) = new BinOp((yystack_[2].value.expType), "-", (yystack_[0].value.expType));}
#line 840 "/home/jiawei/Dropbox/Graduate/COMPILERS/repo/src/parser/parser.cpp" // lalr1.cc:859
    break;

  case 44:
#line 188 "/home/jiawei/Dropbox/Graduate/COMPILERS/repo/src/parser/parser.y" // lalr1.cc:859
    {(yylhs.value.expType) = new BinOp((yystack_[2].value.expType), "*", (yystack_[0].value.expType));}
#line 846 "/home/jiawei/Dropbox/Graduate/COMPILERS/repo/src/parser/parser.cpp" // lalr1.cc:859
    break;

  case 45:
#line 189 "/home/jiawei/Dropbox/Graduate/COMPILERS/repo/src/parser/parser.y" // lalr1.cc:859
    {(yylhs.value.expType) = new BinOp((yystack_[2].value.expType), "/", (yystack_[0].value.expType));}
#line 852 "/home/jiawei/Dropbox/Graduate/COMPILERS/repo/src/parser/parser.cpp" // lalr1.cc:859
    break;

  case 46:
#line 192 "/home/jiawei/Dropbox/Graduate/COMPILERS/repo/src/parser/parser.y" // lalr1.cc:859
    {(yylhs.value.expType) = new UaryOp("!", (yystack_[0].value.expType));}
#line 858 "/home/jiawei/Dropbox/Graduate/COMPILERS/repo/src/parser/parser.cpp" // lalr1.cc:859
    break;

  case 47:
#line 193 "/home/jiawei/Dropbox/Graduate/COMPILERS/repo/src/parser/parser.y" // lalr1.cc:859
    {(yylhs.value.expType) = new UaryOp("-", (yystack_[0].value.expType));}
#line 864 "/home/jiawei/Dropbox/Graduate/COMPILERS/repo/src/parser/parser.cpp" // lalr1.cc:859
    break;

  case 48:
#line 196 "/home/jiawei/Dropbox/Graduate/COMPILERS/repo/src/parser/parser.y" // lalr1.cc:859
    {(yylhs.value.expType) = new Num(*(yystack_[0].value.str));}
#line 870 "/home/jiawei/Dropbox/Graduate/COMPILERS/repo/src/parser/parser.cpp" // lalr1.cc:859
    break;

  case 49:
#line 199 "/home/jiawei/Dropbox/Graduate/COMPILERS/repo/src/parser/parser.y" // lalr1.cc:859
    {(yylhs.value.strType) = new Str{*(yystack_[0].value.str)};}
#line 876 "/home/jiawei/Dropbox/Graduate/COMPILERS/repo/src/parser/parser.cpp" // lalr1.cc:859
    break;

  case 50:
#line 202 "/home/jiawei/Dropbox/Graduate/COMPILERS/repo/src/parser/parser.y" // lalr1.cc:859
    {(yylhs.value.varType) = new Var(*(yystack_[0].value.str));}
#line 882 "/home/jiawei/Dropbox/Graduate/COMPILERS/repo/src/parser/parser.cpp" // lalr1.cc:859
    break;

  case 51:
#line 205 "/home/jiawei/Dropbox/Graduate/COMPILERS/repo/src/parser/parser.y" // lalr1.cc:859
    {(yylhs.value.globidType) = new Globid(*(yystack_[0].value.str));}
#line 888 "/home/jiawei/Dropbox/Graduate/COMPILERS/repo/src/parser/parser.cpp" // lalr1.cc:859
    break;

  case 52:
#line 208 "/home/jiawei/Dropbox/Graduate/COMPILERS/repo/src/parser/parser.y" // lalr1.cc:859
    {(yylhs.value.tdeclType) = new NType{"int"};}
#line 894 "/home/jiawei/Dropbox/Graduate/COMPILERS/repo/src/parser/parser.cpp" // lalr1.cc:859
    break;

  case 53:
#line 209 "/home/jiawei/Dropbox/Graduate/COMPILERS/repo/src/parser/parser.y" // lalr1.cc:859
    {(yylhs.value.tdeclType) = new NType{"cint"};}
#line 900 "/home/jiawei/Dropbox/Graduate/COMPILERS/repo/src/parser/parser.cpp" // lalr1.cc:859
    break;

  case 54:
#line 210 "/home/jiawei/Dropbox/Graduate/COMPILERS/repo/src/parser/parser.y" // lalr1.cc:859
    {(yylhs.value.tdeclType) = new NType{"float"};}
#line 906 "/home/jiawei/Dropbox/Graduate/COMPILERS/repo/src/parser/parser.cpp" // lalr1.cc:859
    break;

  case 55:
#line 211 "/home/jiawei/Dropbox/Graduate/COMPILERS/repo/src/parser/parser.y" // lalr1.cc:859
    {(yylhs.value.tdeclType) = new NType{"sfloat"};}
#line 912 "/home/jiawei/Dropbox/Graduate/COMPILERS/repo/src/parser/parser.cpp" // lalr1.cc:859
    break;

  case 56:
#line 212 "/home/jiawei/Dropbox/Graduate/COMPILERS/repo/src/parser/parser.y" // lalr1.cc:859
    {(yylhs.value.tdeclType) = new NType{"void"};}
#line 918 "/home/jiawei/Dropbox/Graduate/COMPILERS/repo/src/parser/parser.cpp" // lalr1.cc:859
    break;

  case 57:
#line 213 "/home/jiawei/Dropbox/Graduate/COMPILERS/repo/src/parser/parser.y" // lalr1.cc:859
    {(yystack_[0].value.tdeclType)->setRef(); (yylhs.value.tdeclType) = (yystack_[0].value.tdeclType);}
#line 924 "/home/jiawei/Dropbox/Graduate/COMPILERS/repo/src/parser/parser.cpp" // lalr1.cc:859
    break;

  case 58:
#line 214 "/home/jiawei/Dropbox/Graduate/COMPILERS/repo/src/parser/parser.y" // lalr1.cc:859
    {(yystack_[0].value.tdeclType)->setNoalias(); (yylhs.value.tdeclType) = (yystack_[0].value.tdeclType);}
#line 930 "/home/jiawei/Dropbox/Graduate/COMPILERS/repo/src/parser/parser.cpp" // lalr1.cc:859
    break;

  case 59:
#line 217 "/home/jiawei/Dropbox/Graduate/COMPILERS/repo/src/parser/parser.y" // lalr1.cc:859
    {(yystack_[2].value.tdeclsType)->push_back((yystack_[0].value.tdeclType)); (yylhs.value.tdeclsType) = (yystack_[2].value.tdeclsType);}
#line 936 "/home/jiawei/Dropbox/Graduate/COMPILERS/repo/src/parser/parser.cpp" // lalr1.cc:859
    break;

  case 60:
#line 218 "/home/jiawei/Dropbox/Graduate/COMPILERS/repo/src/parser/parser.y" // lalr1.cc:859
    {(yylhs.value.tdeclsType) = new TypeList(); (yylhs.value.tdeclsType)->push_back((yystack_[0].value.tdeclType));}
#line 942 "/home/jiawei/Dropbox/Graduate/COMPILERS/repo/src/parser/parser.cpp" // lalr1.cc:859
    break;

  case 61:
#line 221 "/home/jiawei/Dropbox/Graduate/COMPILERS/repo/src/parser/parser.y" // lalr1.cc:859
    {(yystack_[2].value.vdeclsType)->push_back((yystack_[0].value.vdeclType)); (yylhs.value.vdeclsType) = (yystack_[2].value.vdeclsType);}
#line 948 "/home/jiawei/Dropbox/Graduate/COMPILERS/repo/src/parser/parser.cpp" // lalr1.cc:859
    break;

  case 62:
#line 222 "/home/jiawei/Dropbox/Graduate/COMPILERS/repo/src/parser/parser.y" // lalr1.cc:859
    {(yylhs.value.vdeclsType) = new VarList(); (yylhs.value.vdeclsType)->push_back((yystack_[0].value.vdeclType));}
#line 954 "/home/jiawei/Dropbox/Graduate/COMPILERS/repo/src/parser/parser.cpp" // lalr1.cc:859
    break;

  case 63:
#line 225 "/home/jiawei/Dropbox/Graduate/COMPILERS/repo/src/parser/parser.y" // lalr1.cc:859
    {(yylhs.value.vdeclType) = new VarDecl((yystack_[1].value.tdeclType), (yystack_[0].value.varType));}
#line 960 "/home/jiawei/Dropbox/Graduate/COMPILERS/repo/src/parser/parser.cpp" // lalr1.cc:859
    break;


#line 964 "/home/jiawei/Dropbox/Graduate/COMPILERS/repo/src/parser/parser.cpp" // lalr1.cc:859
            default:
              break;
            }
        }
      catch (const syntax_error& yyexc)
        {
          error (yyexc);
          YYERROR;
        }
      YY_SYMBOL_PRINT ("-> $$ =", yylhs);
      yypop_ (yylen);
      yylen = 0;
      YY_STACK_PRINT ();

      // Shift the result of the reduction.
      yypush_ (YY_NULLPTR, yylhs);
    }
    goto yynewstate;

  /*--------------------------------------.
  | yyerrlab -- here on detecting error.  |
  `--------------------------------------*/
  yyerrlab:
    // If not already recovering from an error, report this error.
    if (!yyerrstatus_)
      {
        ++yynerrs_;
        error (yysyntax_error_ (yystack_[0].state, yyla));
      }


    if (yyerrstatus_ == 3)
      {
        /* If just tried and failed to reuse lookahead token after an
           error, discard it.  */

        // Return failure if at end of input.
        if (yyla.type_get () == yyeof_)
          YYABORT;
        else if (!yyla.empty ())
          {
            yy_destroy_ ("Error: discarding", yyla);
            yyla.clear ();
          }
      }

    // Else will try to reuse lookahead token after shifting the error token.
    goto yyerrlab1;


  /*---------------------------------------------------.
  | yyerrorlab -- error raised explicitly by YYERROR.  |
  `---------------------------------------------------*/
  yyerrorlab:

    /* Pacify compilers like GCC when the user code never invokes
       YYERROR and the label yyerrorlab therefore never appears in user
       code.  */
    if (false)
      goto yyerrorlab;
    /* Do not reclaim the symbols of the rule whose action triggered
       this YYERROR.  */
    yypop_ (yylen);
    yylen = 0;
    goto yyerrlab1;

  /*-------------------------------------------------------------.
  | yyerrlab1 -- common code for both syntax error and YYERROR.  |
  `-------------------------------------------------------------*/
  yyerrlab1:
    yyerrstatus_ = 3;   // Each real token shifted decrements this.
    {
      stack_symbol_type error_token;
      for (;;)
        {
          yyn = yypact_[yystack_[0].state];
          if (!yy_pact_value_is_default_ (yyn))
            {
              yyn += yyterror_;
              if (0 <= yyn && yyn <= yylast_ && yycheck_[yyn] == yyterror_)
                {
                  yyn = yytable_[yyn];
                  if (0 < yyn)
                    break;
                }
            }

          // Pop the current state because it cannot handle the error token.
          if (yystack_.size () == 1)
            YYABORT;

          yy_destroy_ ("Error: popping", yystack_[0]);
          yypop_ ();
          YY_STACK_PRINT ();
        }


      // Shift the error token.
      error_token.state = yyn;
      yypush_ ("Shifting", error_token);
    }
    goto yynewstate;

    // Accept.
  yyacceptlab:
    yyresult = 0;
    goto yyreturn;

    // Abort.
  yyabortlab:
    yyresult = 1;
    goto yyreturn;

  yyreturn:
    if (!yyla.empty ())
      yy_destroy_ ("Cleanup: discarding lookahead", yyla);

    /* Do not reclaim the symbols of the rule whose action triggered
       this YYABORT or YYACCEPT.  */
    yypop_ (yylen);
    while (1 < yystack_.size ())
      {
        yy_destroy_ ("Cleanup: popping", yystack_[0]);
        yypop_ ();
      }

    return yyresult;
  }
    catch (...)
      {
        YYCDEBUG << "Exception caught: cleaning lookahead and stack"
                 << std::endl;
        // Do not try to display the values of the reclaimed symbols,
        // as their printer might throw an exception.
        if (!yyla.empty ())
          yy_destroy_ (YY_NULLPTR, yyla);

        while (1 < yystack_.size ())
          {
            yy_destroy_ (YY_NULLPTR, yystack_[0]);
            yypop_ ();
          }
        throw;
      }
  }

  void
  Parser::error (const syntax_error& yyexc)
  {
    error (yyexc.what());
  }

  // Generate an error message.
  std::string
  Parser::yysyntax_error_ (state_type yystate, const symbol_type& yyla) const
  {
    // Number of reported tokens (one for the "unexpected", one per
    // "expected").
    size_t yycount = 0;
    // Its maximum.
    enum { YYERROR_VERBOSE_ARGS_MAXIMUM = 5 };
    // Arguments of yyformat.
    char const *yyarg[YYERROR_VERBOSE_ARGS_MAXIMUM];

    /* There are many possibilities here to consider:
       - If this state is a consistent state with a default action, then
         the only way this function was invoked is if the default action
         is an error action.  In that case, don't check for expected
         tokens because there are none.
       - The only way there can be no lookahead present (in yyla) is
         if this state is a consistent state with a default action.
         Thus, detecting the absence of a lookahead is sufficient to
         determine that there is no unexpected or expected token to
         report.  In that case, just report a simple "syntax error".
       - Don't assume there isn't a lookahead just because this state is
         a consistent state with a default action.  There might have
         been a previous inconsistent state, consistent state with a
         non-default action, or user semantic action that manipulated
         yyla.  (However, yyla is currently not documented for users.)
       - Of course, the expected token list depends on states to have
         correct lookahead information, and it depends on the parser not
         to perform extra reductions after fetching a lookahead from the
         scanner and before detecting a syntax error.  Thus, state
         merging (from LALR or IELR) and default reductions corrupt the
         expected token list.  However, the list is correct for
         canonical LR with one exception: it will still contain any
         token that will not be accepted due to an error action in a
         later state.
    */
    if (!yyla.empty ())
      {
        int yytoken = yyla.type_get ();
        yyarg[yycount++] = yytname_[yytoken];
        int yyn = yypact_[yystate];
        if (!yy_pact_value_is_default_ (yyn))
          {
            /* Start YYX at -YYN if negative to avoid negative indexes in
               YYCHECK.  In other words, skip the first -YYN actions for
               this state because they are default actions.  */
            int yyxbegin = yyn < 0 ? -yyn : 0;
            // Stay within bounds of both yycheck and yytname.
            int yychecklim = yylast_ - yyn + 1;
            int yyxend = yychecklim < yyntokens_ ? yychecklim : yyntokens_;
            for (int yyx = yyxbegin; yyx < yyxend; ++yyx)
              if (yycheck_[yyx + yyn] == yyx && yyx != yyterror_
                  && !yy_table_value_is_error_ (yytable_[yyx + yyn]))
                {
                  if (yycount == YYERROR_VERBOSE_ARGS_MAXIMUM)
                    {
                      yycount = 1;
                      break;
                    }
                  else
                    yyarg[yycount++] = yytname_[yyx];
                }
          }
      }

    char const* yyformat = YY_NULLPTR;
    switch (yycount)
      {
#define YYCASE_(N, S)                         \
        case N:                               \
          yyformat = S;                       \
        break
        YYCASE_(0, YY_("syntax error"));
        YYCASE_(1, YY_("syntax error, unexpected %s"));
        YYCASE_(2, YY_("syntax error, unexpected %s, expecting %s"));
        YYCASE_(3, YY_("syntax error, unexpected %s, expecting %s or %s"));
        YYCASE_(4, YY_("syntax error, unexpected %s, expecting %s or %s or %s"));
        YYCASE_(5, YY_("syntax error, unexpected %s, expecting %s or %s or %s or %s"));
#undef YYCASE_
      }

    std::string yyres;
    // Argument number.
    size_t yyi = 0;
    for (char const* yyp = yyformat; *yyp; ++yyp)
      if (yyp[0] == '%' && yyp[1] == 's' && yyi < yycount)
        {
          yyres += yytnamerr_ (yyarg[yyi++]);
          ++yyp;
        }
      else
        yyres += *yyp;
    return yyres;
  }


  const signed char Parser::yypact_ninf_ = -53;

  const signed char Parser::yytable_ninf_ = -1;

  const short int
  Parser::yypact_[] =
  {
      41,   220,   220,    10,    41,   -53,   -11,   -53,   -53,   -53,
     -53,   -53,   -53,   220,   220,    13,    13,   -53,   -53,   -11,
     -53,   -53,   -53,   -53,    -4,     0,   280,   285,    -8,    12,
     -24,   -53,     8,   -53,   -20,    54,   -53,    47,   -53,    -8,
     220,   -53,    20,   220,   -53,   224,   224,    32,    34,   215,
      18,   224,   -53,   -53,    89,   -53,   121,   -53,   -53,   -53,
     -53,    36,    83,   -53,   -53,   -53,   -53,   -53,   312,    67,
     224,   224,   -53,   133,    53,   -53,   152,   249,   -53,   -53,
     224,   224,   224,   224,   224,   224,   224,   224,   224,   224,
     -53,   212,   224,   261,   275,   -53,   -53,   -53,   -53,   312,
     141,   -53,   -53,    67,    67,    43,    43,   333,   323,   -53,
      -6,   312,   164,   180,   180,   -53,   224,   -53,    69,   -53,
     312,   180,   -53
  };

  const unsigned char
  Parser::yydefact_[] =
  {
       4,     0,     0,     0,     0,     6,     3,    10,    52,    53,
      54,    55,    56,     0,     0,     0,     0,     1,     5,     2,
       9,    58,    57,    51,     0,     0,     0,     0,     0,     0,
       0,    62,     0,    60,     0,     0,    12,     0,    63,     0,
       0,     8,     0,     0,    48,     0,     0,     0,     0,     0,
       0,     0,    14,    17,     0,    16,     0,    30,    31,    32,
      33,     0,     0,    50,    11,    61,     7,    59,    46,    47,
       0,     0,    49,     0,     0,    19,     0,     0,    13,    15,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
      21,     0,     0,     0,     0,    25,    26,    18,    29,    36,
      39,    44,    45,    42,    43,    40,    41,    38,    37,    35,
       0,    28,     0,     0,     0,    34,     0,    20,    24,    22,
      27,     0,    23
  };

  const signed char
  Parser::yypgoto_[] =
  {
     -53,   -53,   -53,    92,    93,     3,   -15,   -53,   -52,   -53,
     -45,   -53,   -53,   -53,   -53,    70,    65,     6,   -53,   -53,
     -23
  };

  const signed char
  Parser::yydefgoto_[] =
  {
      -1,     3,     4,     5,     6,     7,    53,    54,    55,   110,
      56,    57,    58,    59,    74,    60,    61,    29,    34,    30,
      62
  };

  const unsigned char
  Parser::yytable_[] =
  {
      68,    69,    79,    31,    73,    76,    77,    15,    16,    20,
      17,    39,     1,    36,    40,    42,    23,    65,    43,    21,
      22,    23,    20,    44,    64,    93,    94,    45,    35,   115,
      26,    46,   116,    33,    27,    99,   100,   101,   102,   103,
     104,   105,   106,   107,   108,    37,   111,   112,    41,    67,
      63,    37,    51,    82,    83,    84,    85,    23,    75,    44,
      66,   118,   119,    45,     1,     2,    70,    46,    71,   122,
      91,   120,     8,     9,    10,    11,    12,    82,    83,    47,
      24,    25,    48,    49,    50,    13,    14,    37,    51,    92,
      35,    52,    23,    96,    44,   121,    18,    19,    45,    38,
       0,     0,    46,     0,     0,     0,     0,     8,     9,    10,
      11,    12,     0,     0,    47,     0,     0,    48,    49,    50,
      13,    14,    37,    51,     0,    35,    78,    80,    81,     0,
       0,    82,    83,    84,    85,    86,    87,    88,    89,    80,
      81,     0,     0,    82,    83,    84,    85,    86,    87,    88,
      89,    82,    83,    84,    85,    86,    87,     0,    80,    81,
       0,    90,    82,    83,    84,    85,    86,    87,    88,    89,
      80,    81,     0,    95,    82,    83,    84,    85,    86,    87,
      88,    89,     0,    23,     0,    44,     0,     0,     0,    45,
       0,     0,    97,    46,     0,     0,     0,     0,     8,     9,
      10,    11,    12,     0,   117,    47,     0,     0,    48,    49,
      50,    13,    14,    37,    51,    23,    35,    44,    23,    72,
      44,    45,     0,     0,    45,    46,     0,    23,    46,    44,
       0,     0,     0,    45,     0,     0,     0,    46,     8,     9,
      10,    11,    12,     0,     0,    37,    51,   109,    37,    51,
       0,    13,    14,     0,     0,    80,    81,    37,    51,    82,
      83,    84,    85,    86,    87,    88,    89,    80,    81,     0,
       0,    82,    83,    84,    85,    86,    87,    88,    89,     0,
       0,    80,    81,     0,    98,    82,    83,    84,    85,    86,
      87,    88,    89,     0,     0,     0,   113,     0,     8,     9,
      10,    11,    12,     8,     9,    10,    11,    12,     0,     0,
     114,    13,    14,     0,     0,    28,    13,    14,    80,    81,
      32,     0,    82,    83,    84,    85,    86,    87,    88,    89,
      81,     0,     0,    82,    83,    84,    85,    86,    87,    88,
      81,     0,     0,    82,    83,    84,    85,    86,    87
  };

  const signed char
  Parser::yycheck_[] =
  {
      45,    46,    54,    26,    49,    50,    51,     1,     2,     6,
       0,    35,    23,    28,    38,    35,     3,    40,    38,    13,
      14,     3,    19,     5,    39,    70,    71,     9,    36,    35,
      34,    13,    38,    27,    34,    80,    81,    82,    83,    84,
      85,    86,    87,    88,    89,    33,    91,    92,    40,    43,
       3,    33,    34,    10,    11,    12,    13,     3,    40,     5,
      40,   113,   114,     9,    23,    24,    34,    13,    34,   121,
      34,   116,    18,    19,    20,    21,    22,    10,    11,    25,
      15,    16,    28,    29,    30,    31,    32,    33,    34,     6,
      36,    37,     3,    40,     5,    26,     4,     4,     9,    29,
      -1,    -1,    13,    -1,    -1,    -1,    -1,    18,    19,    20,
      21,    22,    -1,    -1,    25,    -1,    -1,    28,    29,    30,
      31,    32,    33,    34,    -1,    36,    37,     6,     7,    -1,
      -1,    10,    11,    12,    13,    14,    15,    16,    17,     6,
       7,    -1,    -1,    10,    11,    12,    13,    14,    15,    16,
      17,    10,    11,    12,    13,    14,    15,    -1,     6,     7,
      -1,    40,    10,    11,    12,    13,    14,    15,    16,    17,
       6,     7,    -1,    40,    10,    11,    12,    13,    14,    15,
      16,    17,    -1,     3,    -1,     5,    -1,    -1,    -1,     9,
      -1,    -1,    40,    13,    -1,    -1,    -1,    -1,    18,    19,
      20,    21,    22,    -1,    40,    25,    -1,    -1,    28,    29,
      30,    31,    32,    33,    34,     3,    36,     5,     3,     4,
       5,     9,    -1,    -1,     9,    13,    -1,     3,    13,     5,
      -1,    -1,    -1,     9,    -1,    -1,    -1,    13,    18,    19,
      20,    21,    22,    -1,    -1,    33,    34,    35,    33,    34,
      -1,    31,    32,    -1,    -1,     6,     7,    33,    34,    10,
      11,    12,    13,    14,    15,    16,    17,     6,     7,    -1,
      -1,    10,    11,    12,    13,    14,    15,    16,    17,    -1,
      -1,     6,     7,    -1,    35,    10,    11,    12,    13,    14,
      15,    16,    17,    -1,    -1,    -1,    35,    -1,    18,    19,
      20,    21,    22,    18,    19,    20,    21,    22,    -1,    -1,
      35,    31,    32,    -1,    -1,    35,    31,    32,     6,     7,
      35,    -1,    10,    11,    12,    13,    14,    15,    16,    17,
       7,    -1,    -1,    10,    11,    12,    13,    14,    15,    16,
       7,    -1,    -1,    10,    11,    12,    13,    14,    15
  };

  const unsigned char
  Parser::yystos_[] =
  {
       0,    23,    24,    42,    43,    44,    45,    46,    18,    19,
      20,    21,    22,    31,    32,    58,    58,     0,    44,    45,
      46,    58,    58,     3,    57,    57,    34,    34,    35,    58,
      60,    61,    35,    58,    59,    36,    47,    33,    56,    35,
      38,    40,    35,    38,     5,     9,    13,    25,    28,    29,
      30,    34,    37,    47,    48,    49,    51,    52,    53,    54,
      56,    57,    61,     3,    47,    61,    40,    58,    51,    51,
      34,    34,     4,    51,    55,    40,    51,    51,    37,    49,
       6,     7,    10,    11,    12,    13,    14,    15,    16,    17,
      40,    34,     6,    51,    51,    40,    40,    40,    35,    51,
      51,    51,    51,    51,    51,    51,    51,    51,    51,    35,
      50,    51,    51,    35,    35,    35,    38,    40,    49,    49,
      51,    26,    49
  };

  const unsigned char
  Parser::yyr1_[] =
  {
       0,    41,    42,    42,    42,    43,    43,    44,    44,    45,
      45,    46,    46,    47,    47,    48,    48,    49,    49,    49,
      49,    49,    49,    49,    49,    49,    49,    50,    50,    51,
      51,    51,    51,    51,    51,    51,    52,    52,    52,    52,
      52,    52,    52,    52,    52,    52,    53,    53,    54,    55,
      56,    57,    58,    58,    58,    58,    58,    58,    58,    59,
      59,    60,    60,    61
  };

  const unsigned char
  Parser::yyr2_[] =
  {
       0,     2,     2,     1,     0,     2,     1,     7,     6,     2,
       1,     7,     6,     3,     2,     2,     1,     1,     3,     2,
       4,     2,     5,     7,     5,     3,     3,     3,     1,     3,
       1,     1,     1,     1,     4,     3,     3,     3,     3,     3,
       3,     3,     3,     3,     3,     3,     2,     2,     1,     1,
       2,     1,     1,     1,     1,     1,     1,     2,     2,     3,
       1,     3,     1,     2
  };



  // YYTNAME[SYMBOL-NUM] -- String name of the symbol SYMBOL-NUM.
  // First, the terminals, then, starting at \a yyntokens_, nonterminals.
  const char*
  const Parser::yytname_[] =
  {
  "$end", "error", "$undefined", "IDENT", "STR", "NUM", "\"=\"", "\"==\"",
  "\"!=\"", "\"!\"", "\"*\"", "\"/\"", "\"+\"", "\"-\"", "\">\"", "\"<\"",
  "\"&&\"", "\"||\"", "\"int\"", "\"cint\"", "\"float\"", "\"sfloat\"",
  "\"void\"", "\"def\"", "\"extern\"", "\"if\"", "\"else\"", "\"for\"",
  "\"while\"", "\"print\"", "\"return\"", "\"noalias\"", "\"ref\"",
  "\"&\"", "\"(\"", "\")\"", "\"{\"", "\"}\"", "\",\"", "\".\"", "\";\"",
  "$accept", "prog", "externs", "extern", "funcs", "func", "blk", "stmts",
  "stmt", "exps", "exp", "binop", "uop", "lit", "slit", "var", "globid",
  "type", "tdecls", "vdecls", "vdecl", YY_NULLPTR
  };

#if YYDEBUG
  const unsigned char
  Parser::yyrline_[] =
  {
       0,   117,   117,   118,   119,   122,   123,   127,   128,   131,
     132,   135,   136,   139,   140,   143,   144,   147,   148,   149,
     150,   151,   152,   153,   154,   155,   156,   159,   160,   163,
     164,   165,   166,   167,   168,   169,   180,   181,   182,   183,
     184,   185,   186,   187,   188,   189,   192,   193,   196,   199,
     202,   205,   208,   209,   210,   211,   212,   213,   214,   217,
     218,   221,   222,   225
  };

  // Print the state stack on the debug stream.
  void
  Parser::yystack_print_ ()
  {
    *yycdebug_ << "Stack now";
    for (stack_type::const_iterator
           i = yystack_.begin (),
           i_end = yystack_.end ();
         i != i_end; ++i)
      *yycdebug_ << ' ' << i->state;
    *yycdebug_ << std::endl;
  }

  // Report on the debug stream that the rule \a yyrule is going to be reduced.
  void
  Parser::yy_reduce_print_ (int yyrule)
  {
    unsigned int yylno = yyrline_[yyrule];
    int yynrhs = yyr2_[yyrule];
    // Print the symbols being reduced, and their result.
    *yycdebug_ << "Reducing stack by rule " << yyrule - 1
               << " (line " << yylno << "):" << std::endl;
    // The symbols being reduced.
    for (int yyi = 0; yyi < yynrhs; yyi++)
      YY_SYMBOL_PRINT ("   $" << yyi + 1 << " =",
                       yystack_[(yynrhs) - (yyi + 1)]);
  }
#endif // YYDEBUG

  // Symbol number corresponding to token number t.
  inline
  Parser::token_number_type
  Parser::yytranslate_ (int t)
  {
    static
    const token_number_type
    translate_table[] =
    {
     0,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     1,     2,     3,     4,
       5,     6,     7,     8,     9,    10,    11,    12,    13,    14,
      15,    16,    17,    18,    19,    20,    21,    22,    23,    24,
      25,    26,    27,    28,    29,    30,    31,    32,    33,    34,
      35,    36,    37,    38,    39,    40
    };
    const unsigned int user_token_number_max_ = 295;
    const token_number_type undef_token_ = 2;

    if (static_cast<int>(t) <= yyeof_)
      return yyeof_;
    else if (static_cast<unsigned int> (t) <= user_token_number_max_)
      return translate_table[t];
    else
      return undef_token_;
  }

#line 7 "/home/jiawei/Dropbox/Graduate/COMPILERS/repo/src/parser/parser.y" // lalr1.cc:1167
} // bison
#line 1505 "/home/jiawei/Dropbox/Graduate/COMPILERS/repo/src/parser/parser.cpp" // lalr1.cc:1167
#line 227 "/home/jiawei/Dropbox/Graduate/COMPILERS/repo/src/parser/parser.y" // lalr1.cc:1168


#include "lexer.h"

static int yylex(bison::Parser::semantic_type *yylval,
                 Lexer &lexer) {
    return lexer.yylex(yylval);
}

void
bison::Parser::error( const string &err_message )
{
   std::cerr << "Error: " << err_message << "\n";
}
